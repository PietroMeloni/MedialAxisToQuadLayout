/**
 @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#include "load_save_trimesh.h"



//using namespace std;


void loadObj(const char     * filename,
              std::vector<double> & xyz,
              std::vector<int>    & tri)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : load_OBJ() : couldn't open input file " << filename << std::endl;
        exit(-1);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        std::string token;
        iss >> token;
        if (token.size() > 1) continue; // vn,fn  .... I don't care

        if (token[0] == 'v')
        {
            double x, y, z;
            iss >> x >> y >> z;
            xyz.push_back(x);
            xyz.push_back(y);
            xyz.push_back(z);
            //cout << "v " << x << " " << y << " " << z << endl;
        }
        else if (token[0] == 'f')
        {
            int v0, v1, v2;
            iss >> v0 >> v1 >> v2;
            tri.push_back(v0-1);
            tri.push_back(v1-1);
            tri.push_back(v2-1);
            //cout << "f " << v0 << " " << v1 << " " << v2 << endl;
        }
    }
    file.close();
}

void saveObj(const char *filename, const std::vector<double> &xyz, const std::vector<int> &tri)
{
    std::ofstream fp;
    fp.open (filename);
    fp.precision(6);
    fp.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

    if(!fp)
    {
        std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : save_OBJ() : couldn't open output file " << filename << std::endl;
        exit(-1);
    }

    //cout << "v " << xyz[3] << " " << xyz[4] << " " << xyz[5] << endl;

    for(int i=0; i<(int)xyz.size(); i+=3)
    {
        fp << "v " << xyz[i] << " " << xyz[i+1] << " " << xyz[i+2] << std::endl;
    }

    for(int i=0; i<(int)tri.size(); i+=3)
    {
        fp << "f " << tri[i]+1 << " " << tri[i+1]+1 << " " << tri[i+2]+1 << std::endl;
    }

    fp.close();
}

void objToOff(const char *f1, const char *f2)
{
    std::ifstream fObj;
    std::ofstream fOff;
    std::istringstream iss;
    std::ostringstream oss;
    std::string line;
    std::string temp;
    int n;

    int nVertices=0;
    int nFaces=0;

    fObj.open(f1);
    fOff.open(f2);

    fOff<<"OFF"<<std::endl;

    while(std::getline(fObj,line))
    {
        iss=std::istringstream(line);

        iss>>temp;

        if(temp[0]=='v')
        {
            nVertices++;
            getline(iss,temp);
            oss<<temp<<" "<<std::endl;
        }
        if(temp[0]=='f')
        {
            nFaces++;
            iss>>n;
            oss<<"3 "<<n-1<<" ";
            iss>>n;
            oss<<n-1<<" ";
            iss>>n;
            oss<<n-1<<" "<<std::endl;
        }

    }
    fOff<<nVertices<<" "<<nFaces<<" 0"<<std::endl;

    fOff<<oss.str();

    fObj.close();
    fOff.close();
}

void offToObj(const char *f1, const char *f2)
{
    std::ofstream fObj;
    std::ifstream fOff;
    std::string line;
    std::istringstream iss;
    std::string temp;

    int nVertices=-1, nFaces=-1;

    int numFaces;
    double n;

    fOff.open(f1);
    fObj.open(f2);

    while(std::getline(fOff,line))
    {
        iss=std::istringstream(line);
        iss>>temp;

        if((temp.compare("OFF")==0)||(temp[0]=='#')) continue;

        if((nVertices==-1)||(nFaces==-1))
        {
            nVertices=atoi(temp.c_str());
            iss>>nFaces;
            break;
        }
    }

    //std::cout<<nVertices<<" "<<nFaces<<std::endl;
    for(int i=0;i<nVertices;++i)
    {
        getline(fOff,line);
        iss=std::istringstream(line);
        getline(iss,temp);
        fObj<<"v "<<temp<<std::endl;
    }

    for(int i=0;i<nFaces;++i)
    {
        getline(fOff,line);
        iss=std::istringstream(line);

        iss>>numFaces;
        fObj<<"f";
        for(int j=0;j<numFaces;++j)
        {
            iss>>n;
            fObj<<" "<<n+1;
        }
        fObj<<std::endl;
    }

    fObj.close();
    fOff.close();
}
