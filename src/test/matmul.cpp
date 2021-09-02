#include "matmul.h"
#include "../main/mesh.h"
#include "../main/processing_element.h"
#include "../main/matrix.h"

namespace UnitTest
{
    void matrix()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matrix\n";
        bool finished = false;
        Matrix<uint32_t> test(3,4);
        uint32_t tmp[3*4] = {1,2,3,4, 6,3,2,1, 4,3,6,1};
        test.fill(tmp);
        std::cout << test;
        std::cout << test(1,0) << '\n';
        std::cout << "DONE\n\n";
    }

    void request()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::request\n";
        bool finished = false;
        Matrix<uint8_t> in(4,4);
        Matrix<uint32_t> out_b(4,4);
        Matrix<uint32_t> out_c(4,4);
        uint8_t tmp[4*4] = {1,2,3,4, 6,5,2,1, 4,3,6,1, 1,2,6,4};
        in.fill(tmp);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::weight, /* propagate */ true, /* size */ 4, &in, &in, &in, &out_b, &out_c);
        std::cout << "DONE\n\n";
    }

    void matmul_weight()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_weight\n";
        bool finished = false;
        Matrix<uint8_t> in(2,2);
        Matrix<uint8_t> in_zero(2,2);
        Matrix<uint32_t> out_b(2,2);
        Matrix<uint32_t> out_c(2,2);
        uint8_t tmp[2*2] = {1,2,3,4};
        in.fill(tmp);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(2, 2, 1, 1);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::weight, /* propagate */ false, /* size */ 2, &in, &in_zero, &in, &out_b, &out_c);
        mesh.waiting.push_back(req);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            // std::cout << mesh;
        }
        std::cout << "DONE\n\n";
        std::cout << out_b;
        std::cout << out_c;
    }

    void matmul_weight2()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_weight2\n";
        bool finished = false;
        Matrix<uint8_t> in(4,4);
        Matrix<uint8_t> in_zero(4,4);
        Matrix<uint32_t> out_b(4,4);
        Matrix<uint32_t> out_c(4,4);
        uint8_t tmp[4*4] = {10,20,30,40, 11,21,31,41, 12,22,32,42, 13,23,33,43};
        in.fill(tmp);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(2, 2, 2, 2);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::weight, /* propagate */ false, /* size */ 4, &in, &in_zero, &in, &out_b, &out_c);
        mesh.waiting.push_back(req);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            // std::cout << mesh;
        }
        std::cout << "DONE\n\n";
        std::cout << out_b;
        std::cout << out_c;
    }

    void matmul_weight_twice()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_weight_twice\n";
        bool finished = false;
        bool finished2 = false;
        Matrix<uint8_t> in(4,4);
        Matrix<uint8_t> in2(4,4);
        Matrix<uint8_t> in_zero(4,4);
        Matrix<uint32_t> out_b(4,4);
        Matrix<uint32_t> out_c(4,4);
        Matrix<uint32_t> out_b2(4,4);
        Matrix<uint32_t> out_c2(4,4);
        uint8_t tmp[4*4] = {10,20,30,40, 11,21,31,41, 12,22,32,42, 13,23,33,43};
        uint8_t tmp2[4*4] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
        in.fill(tmp);
        in2.fill(tmp2);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(2, 2, 2, 2);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::weight, /* propagate */ false, /* size */ 4, &in, &in_zero, &in, &out_b, &out_c);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req2(&finished2, Stationary::weight, /* propagate */ true, /* size */ 4, &in, &in_zero, &in2, &out_b2, &out_c2);
        mesh.waiting.push_back(req);
        mesh.waiting.push_back(req2);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            std::cout << mesh; // 
        }
        std::cout << "DONE 1\n\n";
        std::cout << out_b;
        std::cout << out_c;

        while (!finished2)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            std::cout << mesh; // 
        }
        std::cout << "DONE 2\n\n";
        std::cout << out_b2;
        std::cout << out_c2;
    }


    void matmul_output()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_output\n";
        bool finished = false;
        Matrix<uint8_t> in(2,2);
        Matrix<uint8_t> in_zero(4,4);
        Matrix<uint32_t> out_b(2,2);
        Matrix<uint32_t> out_c(2,2);
        uint8_t tmp[2*2] = {1,2,3,4};
        in.fill(tmp);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(2, 2, 1, 1);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::output, /* propagate */ false, /* size */ 2, &in, &in, &in_zero, &out_b, &out_c);
        mesh.waiting.push_back(req);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            // std::cout << mesh;
        }
        std::cout << "DONE\n\n";
        std::cout << out_b;
        std::cout << out_c;
    }

    void matmul_output2()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_output2\n";
        bool finished = false;
        Matrix<uint8_t> in(4,4);
        Matrix<uint8_t> in_zero(4,4);
        Matrix<uint32_t> out_b(4,4);
        Matrix<uint32_t> out_c(4,4);
        uint8_t tmp[4*4] = {10,20,30,40, 11,21,31,41, 12,22,32,42, 13,23,33,43};
        in.fill(tmp);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(2, 2, 2, 2);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::output, /* propagate */ false, /* size */ 4, &in, &in, &in_zero, &out_b, &out_c);
        mesh.waiting.push_back(req);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            // std::cout << mesh;
        }
        std::cout << "DONE\n\n";
        std::cout << out_b;
        std::cout << out_c;
    }

    void matmul_output_twice()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_output_twice\n";
        bool finished = false;
        bool finished2 = false;
        Matrix<uint8_t> in(4,4);
        Matrix<uint8_t> in2(4,4);
        Matrix<uint8_t> in_zero(4,4);
        Matrix<uint32_t> out_b(4,4);
        Matrix<uint32_t> out_c(4,4);
        Matrix<uint32_t> out_b2(4,4);
        Matrix<uint32_t> out_c2(4,4);
        uint8_t tmp[4*4] = {10,20,30,40, 11,21,31,41, 12,22,32,42, 13,23,33,43};
        uint8_t tmp2[4*4] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
        in.fill(tmp);
        in2.fill(tmp2);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(2, 2, 2, 2);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::output, /* propagate */ false, /* size */ 4, &in, &in, &in_zero, &out_b, &out_c);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req2(&finished2, Stationary::output, /* propagate */ true, /* size */ 4, &in, &in2, &in_zero, &out_b2, &out_c2);
        mesh.waiting.push_back(req);
        mesh.waiting.push_back(req2);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            std::cout << mesh; // 
        }
        std::cout << "DONE 1\n\n";
        std::cout << out_b;
        std::cout << out_c;

        while (!finished2)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            std::cout << mesh; // 
        }
        std::cout << "DONE 2\n\n";
        std::cout << out_b2;
        std::cout << out_c2;
    }


    void matmul_huge_weight()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_huge_weight\n";
        bool finished = false;
        Matrix<uint8_t> in(64,64);
        Matrix<uint8_t> in_zero(64,64);
        Matrix<uint32_t> out_b(64,64);
        Matrix<uint32_t> out_c(64,64);
        uint8_t tmp[64*64];
        for (int i=0; i<64*64; i++) tmp[i] = i%16;
        in.fill(tmp);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(8, 8, 8, 8);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::weight, /* propagate */ false, /* size */ 64, &in, &in_zero, &in, &out_b, &out_c);
        mesh.waiting.push_back(req);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            // std::cout << mesh;
        }
        std::cout << "DONE\n\n";
        // std::cout << out_b;
        // std::cout << out_c;
    }

        
    void matmul_huge_output()
    {
        std::cout << "\n\n\n*************************\n";
        std::cout << "UnitTest::matmul_huge_output\n";
        bool finished = false;
        Matrix<uint8_t> in(64,64);
        Matrix<uint8_t> in_zero(64,64);
        Matrix<uint32_t> out_b(64,64);
        Matrix<uint32_t> out_c(64,64);
        uint8_t tmp[64*64];
        for (int i=0; i<64*64; i++) tmp[i] = i%16;
        in.fill(tmp);
        std::cout << in;
        Mesh<uint8_t, uint32_t, uint32_t> mesh(8, 8, 8, 8);
        Mesh<uint8_t, uint32_t, uint32_t>::Request req(&finished, Stationary::output, /* propagate */ false, /* size */ 64, &in, &in, &in_zero, &out_b, &out_c);
        mesh.waiting.push_back(req);
        while (!finished)
        {
            std::cout << "\n* Calculate\n";
            mesh.calculate(); 
            // std::cout << mesh;
        }
        std::cout << "DONE\n\n";
        // std::cout << out_b;
        // std::cout << out_c;
    }
}