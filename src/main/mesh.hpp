#include <iostream>
#include <cassert>
#include "mesh.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
Mesh<INPUT_T, OUTPUT_T, ACC_T>::Request::Request(bool* _finished, Stationary _stationary, bool _propagate, uint32_t _size, Matrix<INPUT_T>* _in_a, Matrix<INPUT_T>* _in_b, Matrix<INPUT_T>* _in_d, Matrix<OUTPUT_T>* _out_b, Matrix<OUTPUT_T>* _out_c): 
    finished(_finished),
    preload(_stationary == Stationary::weight ? 0 : _size), 
    progress(0), 
    control({_stationary, _propagate}), 
    size(_size), 
    in_a(_in_a), 
    in_b(_in_b), 
    in_d(_in_d), 
    out_b(_out_b), 
    out_c(_out_c) {}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
Mesh<INPUT_T, OUTPUT_T, ACC_T>::Mesh(uint32_t _rows, uint32_t _columns, uint32_t _tile_rows, uint32_t _tile_columns): 
    clock(0),
    rows(_rows), 
    columns(_columns), 
    tile_rows(_tile_rows), 
    tile_columns(_tile_columns),
    tile_table(Matrix<Tile<INPUT_T, OUTPUT_T, ACC_T>>(_rows, _columns, _tile_rows, _tile_columns)) {}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Mesh<INPUT_T, OUTPUT_T, ACC_T>::calculate()
{
    assert(waiting.size() > 0);
    auto *cur = &waiting[0];

    clock += 1;

    /* filling input_a of the leftmost tiles in this mesh */
    uint32_t i = 0;
    INPUT_T tmp_a[cur->size];
    if (cur->control.stationary == Stationary::weight)
    {
        if (cur->preload < cur->size) // preloading weight, not ready for input
        {
            for (; i < cur->size; i++)
                tmp_a[i] = 0;
        }
        else if (cur->progress < cur->size)
        {
            for (; i < cur->progress + 1; i++)
                tmp_a[i] = cur->in_a->get(cur->progress - i, i);

            for (; i < cur->size; i++)
                tmp_a[i] = 0;
        }
        else if (cur->progress < 2 * cur->size)
        {
            if (waiting.size() > 1 
                && waiting[1].progress < waiting[1].size 
                && waiting[1].control.stationary == Stationary::weight) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; i < next->progress + 1; i++)
                    tmp_a[i] = next->in_a->get(next->progress - i, i);
            }
            for (; i < cur->progress - cur->size + 1; i++)
                tmp_a[i] = 0;

            for (; i < cur->size; i++)
                tmp_a[i] = cur->in_a->get(cur->progress - i, i);
        }
        else
        {
            if (waiting.size() > 1 
                && waiting[1].progress < 2 * waiting[1].size
                && waiting[1].control.stationary == Stationary::weight) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; i < next->progress - next->size + 1; i++)
                    tmp_a[i] = 0;

                for (; i < next->size; i++)
                    tmp_a[i] = next->in_a->get(next->progress - i, i);
            }
            
            for (; i < cur->size; i++)
                tmp_a[i] = 0;
        }
    }
    else if (cur->control.stationary == Stationary::output)
    {
        if (cur->progress < cur->size)
        {
            for (; i < cur->progress + 1; i++)
                tmp_a[i] = cur->in_a->get(i, cur->size - cur->progress + i - 1);

            for (; i < cur->size; i++)
                tmp_a[i] = 0;
        }
        else if (cur->progress < 2 * cur->size)
        {
            if (waiting.size() > 1 
                && waiting[1].progress < waiting[1].size
                && waiting[1].control.stationary == Stationary::output) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; i < next->progress + 1; i++)
                    tmp_a[i] = next->in_a->get(i, next->size - next->progress + i - 1);
            }
            for (; i < cur->progress - cur->size + 1; i++)
                tmp_a[i] = 0;

            for (; i < cur->size; i++)
                tmp_a[i] = cur->in_a->get(i, i - cur->progress + cur->size - 1);
        }
        else
        {
            if (waiting.size() > 1 
                && waiting[1].progress < 2 * waiting[1].size
                && waiting[1].control.stationary == Stationary::output) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; i < next->progress - next->size + 1; i++)
                    tmp_a[i] = 0;

                for (; i < next->size; i++)
                    tmp_a[i] = next->in_a->get(i, i - next->progress + next->size - 1);
            }
            
            for (; i < cur->size; i++)
                tmp_a[i] = 0;
        }
    }

    for (uint32_t i_tile = 0; i_tile < rows; i_tile++)
        tile_table(i_tile,0).input_a(&tmp_a[tile_rows * i_tile]);



    /* filling input_b and input_d of the uppermost tiles in this mesh */
    uint32_t j = 0;
    INPUT_T tmp_b[cur->size];
    INPUT_T tmp_d[cur->size];
    control_t tmp_ctrl[cur->size];
    control_t ctrl_default = {waiting.back().control.stationary, !waiting.back().control.propagate};
    if (cur->control.stationary == Stationary::weight)
    {
        if (cur->preload < cur->size) // preloading weight
        {
            for (; j < cur->preload + 1; j++) {
                tmp_b[j] = cur->in_b->get(cur->size - cur->preload + j - 1, j);
                tmp_d[j] = cur->in_d->get(cur->size - cur->preload + j - 1, j);
                tmp_ctrl[j] = cur->control;
            }
            for (; j < cur->size; j++) {
                tmp_b[j] = 0;
                tmp_d[j] = 0;
                tmp_ctrl[j] = ctrl_default;
            }
        }
        else if (cur->progress < cur->size)
        {
            if (waiting.size() > 1 
                && waiting[1].preload < waiting[1].size
                && waiting[1].control.stationary == Stationary::weight) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; j < next->preload + 1; j++) {
                    tmp_b[j] = next->in_b->get(next->size - next->preload + j - 1, j);
                    tmp_d[j] = next->in_d->get(next->size - next->preload + j - 1, j);
                    tmp_ctrl[j] = next->control;
                }
            }
            for (; j < cur->progress + 1; j++) {
                tmp_b[j] = 0;
                tmp_d[j] = 0;
                tmp_ctrl[j] = ctrl_default;
            }

            for (; j < cur->size; j++) {
                tmp_b[j] = cur->in_b->get(j - cur->progress - 1, j);
                tmp_d[j] = cur->in_d->get(j - cur->progress - 1, j);
                tmp_ctrl[j] = cur->control;
            }
        }
        else
        {
            if (waiting.size() > 1 
                && waiting[1].progress < waiting[1].size
                && waiting[1].control.stationary == Stationary::weight) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; j < next->progress + 1; j++) {
                    tmp_b[j] = 0;
                    tmp_d[j] = 0;
                    tmp_ctrl[j] = ctrl_default;
                }

                for (; j < next->size; j++) {
                    tmp_b[j] = next->in_b->get(j - next->progress - 1, j);
                    tmp_d[j] = next->in_d->get(j - next->progress - 1, j);
                    tmp_ctrl[j] = next->control;
                }
            }
            for (; j < cur->size; j++) {
                tmp_b[j] = 0;
                tmp_d[j] = 0;
                tmp_ctrl[j] = ctrl_default;
            }
        }
    }
    else if (cur->control.stationary == Stationary::output)
    {
        if (cur->progress < cur->size) // 
        {
            for (; j < cur->progress + 1; j++) {
                tmp_b[j] = cur->in_b->get(cur->size - cur->progress + j - 1, j);
                tmp_d[j] = cur->in_d->get(cur->size - cur->progress + j - 1, j);
                tmp_ctrl[j] = cur->control;
            }
            for (; j < cur->size; j++) {
                tmp_b[j] = 0;
                tmp_d[j] = 0;
                tmp_ctrl[j] = ctrl_default;
            }
        }
        else if (cur->progress < 2 * cur->size)
        {
            if (waiting.size() > 1 
                && waiting[1].progress < waiting[1].size
                && waiting[1].control.stationary == Stationary::output) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; j < next->progress + 1; j++) {
                    tmp_b[j] = next->in_b->get(next->size - next->progress + j - 1, j);
                    tmp_d[j] = next->in_d->get(next->size - next->progress + j - 1, j);
                    tmp_ctrl[j] = next->control;
                }
            }
            for (; j < cur->progress - cur->size + 1; j++) {
                tmp_b[j] = 0;
                tmp_d[j] = 0;
                tmp_ctrl[j] = ctrl_default;
            }

            for (; j < cur->size; j++) {
                tmp_b[j] = cur->in_b->get(j - cur->progress + cur->size - 1, j);
                tmp_d[j] = cur->in_d->get(j - cur->progress + cur->size - 1, j);
                tmp_ctrl[j] = cur->control;
            }
        }
        else
        {
            if (waiting.size() > 1 
                && waiting[1].progress < 2 * waiting[1].size
                && waiting[1].control.stationary == Stationary::output) // more calc waiting
            {
                auto *next = &waiting[1];
                for (; j < next->progress - next->size + 1; j++) {
                    tmp_b[j] = 0;
                    tmp_d[j] = 0;
                    tmp_ctrl[j] = ctrl_default;
                }

                for (; j < next->size; j++) {
                    tmp_b[j] = next->in_b->get(j - next->progress + next->size - 1, j);
                    tmp_d[j] = next->in_d->get(j - next->progress + next->size - 1, j);
                    tmp_ctrl[j] = next->control;
                }
            }

            for (; j < cur->size; j++) {
                tmp_b[j] = 0;
                tmp_d[j] = 0;
                tmp_ctrl[j] = ctrl_default;
            }
        }
    }

    for (uint32_t j_tile = 0; j_tile < columns; j_tile++)
    {
        tile_table(0,j_tile).input_b(&tmp_b[tile_columns * j_tile]);
        tile_table(0,j_tile).input_d(&tmp_d[tile_columns * j_tile]);
        tile_table(0,j_tile).input_ctrl(&tmp_ctrl[tile_columns * j_tile]);
    }

    /* filling input_a, input_b and input_d of the other tiles */
    for (int32_t i = rows-1; i >= 0; i--)
    {
        for (int32_t j = columns-1; j >= 0; j--)
        {
            if (j > 0)
            {
                tile_table(i,j).input_a(tile_table(i, j-1).out_a);
            }
            if (i > 0)
            {
                tile_table(i,j).input_b(tile_table(i-1, j).out_b);
                tile_table(i,j).input_d(tile_table(i-1, j).out_c);
                tile_table(i,j).input_ctrl(tile_table(i-1, j).out_ctrl);
            }

            tile_table(i,j).calculate();
        }
    }


    // increase state

    if (cur->preload < cur->size)
        cur->preload += 1;
    else {
        cur->progress += 1;
        if (waiting.size() > 1)
        {
            if (waiting[1].control.stationary == Stationary::weight)
            {
                if (waiting[1].preload < waiting[1].size)
                    waiting[1].preload += 1;
                else
                    waiting[1].progress += 1;
            }
            else if (waiting[1].control.stationary == Stationary::output)
            {
                if (cur->progress > cur->size)
                    waiting[1].progress += 1;
            }
        }
    }


    std::cout << "[Curr] Preload: " << cur->preload << "\tProgress: " << cur->progress << "\n";
    if (waiting.size() > 1)
        std::cout << "[Next] Preload: " << waiting[1].preload << "\tProgress: " << waiting[1].progress << "\n";
    std::cout << '\n';

    // getting output

    if (cur->control.stationary == Stationary::weight)
    {
        if (waiting.size() > 1 && waiting[1].progress >= waiting[1].size)
        {
            auto *next = &waiting[1];
            for (uint32_t i=0; i<next->progress - next->size + 1; i++)
            {
                uint32_t x = next->progress - next->size - i;
                if (x >= next->size) continue;
                if (i >= next->size) continue;
                next->out_b->get(x, i) = tile_table(rows-1, i/tile_rows).out_b[i%tile_rows];
                std::cout << "[Next] Output : " << x << " " << i << " - " << next->out_b->get(x, i) << '\n';
            }
        }

        if (cur->progress >= cur->size)
        {
            uint32_t cnt = 0;
            for (uint32_t i=0; i<cur->progress - cur->size + 1; i++)
            {
                uint32_t x = cur->progress - cur->size - i;
                if (x >= cur->size) continue;
                if (i >= cur->size) continue;
                cnt++;
                cur->out_b->get(x, i) = tile_table(rows-1, i/tile_rows).out_b[i%tile_rows];
                std::cout << "[Curr] Output : " << x << " " << i << " - " << cur->out_b->get(x, i) << '\n';
            }

            if (cnt == 1 && cur->progress > cur->size){
                std::cout << "[+] end of a calculation.\n";
                std::cout << "   consumed clock from the beginning: " << clock << '\n';
                *(cur->finished) = true;
                waiting.erase(waiting.begin());
            }
        }
    }
    else if (cur->control.stationary == Stationary::output)
    {
        if (waiting.size() > 1 && waiting[1].progress > waiting[1].size)
        {
            auto *next = &waiting[1];
            for (uint32_t i=0; i<next->progress - next->size; i++)
            {
                uint32_t x = next->progress - next->size - i - 1;
                if (x >= next->size) continue;
                if (i >= next->size) continue;
                next->out_c->get(x, i) = tile_table(x/tile_rows, i/tile_rows).processing_element_table(x%tile_rows, i%tile_rows).out_c;
                std::cout << "[Next] Output : " << x << " " << i << " - " << next->out_c->get(x, i) << '\n';
            }
        }

        if (cur->progress > cur->size)
        {
            uint32_t cnt = 0;
            for (uint32_t i=0; i<cur->progress - cur->size; i++)
            {
                uint32_t x = cur->progress - cur->size - i - 1;
                if (x >= cur->size) continue;
                if (i >= cur->size) continue;
                cnt++;
                cur->out_c->get(x, i) = tile_table(x/tile_rows, i/tile_rows).processing_element_table(x%tile_rows, i%tile_rows).out_c;
                std::cout << "[Curr] Output : " << x << " " << i << " - " << cur->out_c->get(x, i) << '\n';
            }

            if (cnt == 1 && cur->progress > cur->size + 1){
                std::cout << "[+] end of a calculation.\n";
                std::cout << "   consumed clock from the beginning: " << clock << '\n';
                *(cur->finished) = true;
                waiting.erase(waiting.begin());
            }
        }
    }


}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
std::ostream &operator<<(std::ostream &os, const Mesh<INPUT_T, OUTPUT_T, ACC_T> &mesh)
{
    os << mesh.tile_table;
    return os;
}
