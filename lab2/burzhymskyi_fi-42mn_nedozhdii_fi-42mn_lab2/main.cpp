//
// Created by akhzulag on 04.10.2023.
//
#include "src/gen/generator.h"
#include "test.h"
#include <fstream>
#include <iostream>

int main() {

    srand(time(0));

    Geffe g; L20 l,l1; L89 l89; Wolfram w; Librarian lib; Lehmer leh; BM bm; BSS bs; BuiltinGen build;
    size_t r = 512;
    double alpha = 0.05;
    size_t len = 8*262144;
    uint32_t lehmerX0 = 1 + rand() % ((uint64_t)1 << 32);
    uint32_t l20X0 = 1 + rand() % ((uint64_t)1 << 20);
    uint64_t l89X0 = 1 + rand() % ((uint64_t)1 << 63);
    uint64_t Gx0 = 1 + rand() % ((uint64_t)1 << 11);
    uint64_t Gy0 = 1 + rand() % ((uint64_t)1 << 9);
    uint64_t Gs0 = 1 + rand() % ((uint64_t)1 << 10);
    uint64_t Wx0 = 1 + rand()%((uint64_t)1 << 32);
    std::cout<<"\nBuild: "<<TEST::uniformDistribution(build.generator(len), alpha);
    std::cout<<"\nLehmerLow: "<<TEST::uniformDistribution(leh.generateLow(lehmerX0,len),alpha);
    std::cout<<"\nLehmerLHigh: "<<TEST::uniformDistribution(leh.generateHigh(lehmerX0,len),alpha);
    std::cout<<"\nL20: "<<TEST::uniformDistribution(l.generate(l20X0,len),alpha);
    std::cout<<"\nL89: "<<TEST::uniformDistribution(l89.generate(l89X0,len),alpha);
    std::cout<<"\nGeffe: "<<TEST::uniformDistribution(g.generate(Gx0,Gy0,Gs0,len), alpha);
    std::cout<<"\nWolfram: "<<TEST::uniformDistribution(w.generate(len,Wx0),alpha);
    std::cout<<"\nBM: "<<TEST::uniformDistribution(bm.generate(len), alpha);
    std::cout<<"\nBM256: "<<TEST::uniformDistribution(bm.generate256(len), alpha);
    std::cout<<"\nBBS: "<<TEST::uniformDistribution(bs.generate(len), alpha);
    std::cout<<"\nBBS256: "<<TEST::uniformDistribution(bs.generate256(len),alpha);

    std::ifstream file("../src/gen/MeinKampf.txt"); // Replace "example.txt" with your file path
    std::string fileContent = "";

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line)) {
            fileContent += line; // Append each line to the fileContent string with a newline character
        }
        file.close();
    } else {
        std::cout << "Unable to open the file." << std::endl;
        return 1; // Return an error code to indicate failure
    }


    std::cout<<"\nLibrarian: "<<TEST::uniformDistribution(lib.generate(fileContent),alpha);

    std::cout<<"\n\nIndependence test:\n";

    std::cout<<"\nBuild: "<<TEST::independence(build.generator(len), alpha);
    std::cout<<"\nLehmerLow: "<<TEST::independence(leh.generateLow(lehmerX0,len),alpha);
    std::cout<<"\nLehmerLHigh: "<<TEST::independence(leh.generateHigh(lehmerX0,len),alpha);
    std::cout<<"\nL20: "<<TEST::independence(l.generate(l20X0,len),alpha);
    std::cout<<"\nL89: "<<TEST::independence(l89.generate(l89X0,len),alpha);
    std::cout<<"\nGeffe: "<<TEST::independence(g.generate(Gx0,Gy0,Gs0,len),alpha);
    std::cout<<"\nLibrarian: "<<TEST::independence(lib.generate(fileContent),alpha);
    std::cout<<"\nWolfram: "<<TEST::independence(w.generate(len,Wx0),alpha);
    std::cout<<"\nBM: "<<TEST::independence(bm.generate(len),alpha);
    std::cout<<"\nBM256 "<<TEST::independence(bm.generate256(len),alpha);
    std::cout<<"\nBSS: "<<TEST::independence(bs.generate(len),alpha);
    std::cout<<"\nBSS256: "<<TEST::independence(bs.generate256(len),alpha);





    std::cout<<"\n\nUniformity test:\n";
    std::cout<<"\nBuild: "<<TEST::uniformity(build.generator(len), r, alpha);
    std::cout<<"\nLehmerLow: "<<TEST::uniformity(leh.generateLow(lehmerX0,len),r,alpha);
    std::cout<<"\nLehmerLHigh: "<<TEST::uniformity(leh.generateHigh(lehmerX0,len),r,alpha);
    std::cout<<"\nL20: "<<TEST::uniformity(l.generate(l20X0,len),r,alpha);
    std::cout<<"\nL89: "<<TEST::uniformity(l89.generate(l89X0,len),r,alpha);
    std::cout<<"\nGeffe: "<<TEST::uniformity(g.generate(Gx0,Gy0,Gs0,len),r,alpha);
    std::cout<<"\nLibrarian: "<<TEST::uniformity(lib.generate(fileContent),r,alpha);
    std::cout<<"\nWolfram: "<<TEST::uniformity(w.generate(len,Wx0),r,alpha);
    std::cout<<"\nBM: "<<TEST::uniformity(bm.generate(len),r,alpha);
    std::cout<<"\nBM256: "<<TEST::uniformity(bm.generate256(len),r,alpha);
    std::cout<<"\nBSS: "<<TEST::uniformity(bs.generate(len),r,alpha);
    std::cout<<"\nBSS256: "<<TEST::uniformity(bs.generate256(len),r,alpha);












    return 0;
}
