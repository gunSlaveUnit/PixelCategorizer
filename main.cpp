#include "categorizer.h"

unsigned int Histogram[256] = {0};
int alphabet[256];

int main(int argc, char** argv)
{
    std::fstream f;
    std::ofstream out("hisgoram.txt");
    int width = 0, height = 0;
    char PGMHeader[2];

    if(argc == 1)
    {
        std::cout<<"Error: no arguments were passed to the program.\n";
        return -1;
    }

    for(int i = 1;i<argc;++i)
    {
        f.open(argv[i], std::ios::binary | std::ios::in);

        if(!f.is_open())
        {
            std::cout<<"Error: file "<<argv[i]<<" not available.\n";
            continue;
        }
        else
            std::cout<<"File "<<argv[i]<<" was opened successfully.\n";

        f.read(PGMHeader, 2);
        if(PGMHeader[0]!=0x50||PGMHeader[1]!=0x35)
        {
            std::cout<<"Error: file "<<argv[i]<<" is not a PGM file.\n";
            continue;
        }
        else
            std::cout<<"PGM's header in "<<argv[i]<<" is detected.\n";

        f.seekg(static_cast<unsigned int>(f.tellg())+1);
        unsigned int digit_begin = static_cast<int>(f.tellg());
        char Symbol;
        do
        {
            f.read(&Symbol, 1);
        } while(Symbol!=0x20);
        unsigned int digit_end = static_cast<int>(f.tellg())-2;

        for(int d = '0', j = 0;d<='9';++d, ++j)
            alphabet[d] = j;

        for(unsigned int j = digit_end, c = 1; j >= digit_begin; --j, c*=10)
        {
            f.seekg(j);
            f.read(&Symbol, 1);
            width += alphabet[static_cast<int>(Symbol)] * c;
        }
        std::cout<<"Width of image: "<<width<<std::endl;

        digit_begin = digit_end + 2;
        do
        {
            f.read(&Symbol, 1);
        }while(Symbol!=0x0A);
        digit_end = static_cast<int>(f.tellg())-2;
        for(unsigned int j = digit_end, c = 1; j >= digit_begin; --j, c*=10)
        {
            f.seekg(j);
            f.read(&Symbol, 1);
            height += alphabet[static_cast<unsigned char>(Symbol)] * c;
        }
        std::cout<<"Height of image: "<<height<<std::endl;

        f.seekg(digit_end + 6);
        for(int d = 0;d<width*height;++d)
        {
            if(f.eof())
                break;
            f.read(&Symbol, 1);
            Histogram[static_cast<unsigned char>(Symbol)] += 1;
        }
        std::cout<<std::endl;

        for(int j = 0;j<256;++j)
        {
            std::cout<<"["<<j<<"]";
            for(unsigned int c = 0;c < Histogram[j]; ++c)
                std::cout<<"|";
            std::cout<<std::endl;
        }

        f.close();

        out<<"File: "<<argv[i]<<std::endl;
        out<<"Height: "<<height<<"\t"<<"Width: "<<width<<std::endl;
        for(int j = 0;j<256;++j)
        {
            out<<"["<<j<<"]";
            for(unsigned int c = 0;c < Histogram[j]; ++c)
                out<<"|";
            out<<std::endl;
        }
        out<<"End of file "<<argv[i]<<std::endl<<std::endl;

        out.close();
    }

    return 0;
}
