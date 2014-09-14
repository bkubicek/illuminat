#include "stlfile.h"
#include <QString>
#include <fstream>

#include <iostream>
#include <sstream>
#include <float.h>
#include <stdint.h>

using namespace std;
StlFile::StlFile()
{
}

void StlFile::read(const QString &filename,bool bin)
{

    if(bin)
    {
        //cerr<<"exporting bin"<<endl;
        char header[80]="";
        uint32_t triangles=t.size();

        fstream in(filename.toLatin1(),fstream::in|fstream::binary);
        if(!in.good())
        {
            cerr<<"file not found"<<endl;
            return ;
        }
        in.read(header,80);
        in.read((char*)&triangles,sizeof(uint32_t));

        t.resize(triangles);

        for(int i=0;i<(int)t.size();i++)
        {
            float data[3*4];
            in.read((char*)&data[0],sizeof(data));
            t[i].p[0][0]=data[3];
            t[i].p[0][1]=data[4];
            t[i].p[0][2]=data[5];
            t[i].p[1][0]=data[6];
            t[i].p[1][1]=data[7];
            t[i].p[1][2]=data[8];
            t[i].p[2][0]=data[9];
            t[i].p[2][1]=data[10];
            t[i].p[2][2]=data[11];
            t[i].n[0]=data[0];
            t[i].n[1]=data[1];
            t[i].n[2]=data[2];
            t[i].calcRange();
            t[i].calcN();



            uint16_t d=0;
            in.read((char*)&d,sizeof(d));

        }
        in.close();
    }
    else //ascii
    {
        fstream in(filename.toLatin1(),fstream::in);
        string header;
        getline(in,header);
        string solid;
        getline(in,solid);
        while(in.good())
        {
            string facet;
            getline(in,facet);
            stringstream ss(facet);
            string f;
            ss>>f;
            if(f=="endsolid")
                break;//eof
            string outer;
            getline(in,outer);
            string  vertex;
            TriFace tr;
            in>>vertex;in>>tr.p[0][0];in>>tr.p[0][1];in>>tr.p[0][2];getline(in,vertex);
            in>>vertex;in>>tr.p[1][0];in>>tr.p[1][1];in>>tr.p[1][2];getline(in,vertex);
            in>>vertex;in>>tr.p[2][0];in>>tr.p[2][1];in>>tr.p[2][2];getline(in,vertex);
            string endloop;
           getline(in,endloop);
           getline(in,endloop);
        }
        //out<<"endsolid thing"<<endl;
        //out.close();
    }
    calcRange();
}

void StlFile::write(const QString &filename, bool bin)
{
    if(bin)
    {
        //cerr<<"exporting bin"<<endl;
        char header[80]="foobar";
        uint32_t triangles=t.size();

        fstream out(filename.toLatin1(),fstream::out|fstream::binary);
        out.write(header,80);
        out.write((char*)&triangles,sizeof(uint32_t));



        for(int i=0;i<(int)t.size();i++)
        {
            float data[3*4];
            data[0]=0;data[1]=0;data[2]=0; //n
            data[3]=t[i].p[0][0];
            data[4]=t[i].p[0][1];
            data[5]=t[i].p[0][2];
            data[6]=t[i].p[1][0];
            data[7]=t[i].p[1][1];
            data[8]=t[i].p[1][2];
            data[9]=t[i].p[2][0];
            data[10]=t[i].p[2][1];
            data[11]=t[i].p[2][2];
            out.write((char*)&data[0],sizeof(data));
            uint16_t d=0;
            out.write((char*)&d,sizeof(d));

        }
        out.close();
    }
    else
    {
        cerr<<"exporting Ascii"<<endl;

        fstream out(filename.toLatin1(),fstream::out);
        out<<"solid thing"<<endl;
        for(int i=0;i<(int)t.size();i++)
        {
          out<<"faced normal 0 0 0\nouter loop\n";
          out<<"vertex ";
          out<<t[i].p[0][0]<<" ";
          out<<t[i].p[0][1]<<" ";
          out<<t[i].p[0][2]<<endl;
          out<<"vertex ";
          out<<t[i].p[1][0]<<" ";
          out<<t[i].p[1][1]<<" ";
          out<<t[i].p[1][2]<<endl;
          out<<"vertex ";
          out<<t[i].p[2][0]<<" ";
          out<<t[i].p[2][1]<<" ";
          out<<t[i].p[2][2]<<endl;
          out<<"endloop\nendfacet\n";
        }
        out<<"endsolid thing"<<endl;
        out.close();
    }
}



void StlFile::calcRange()
{
    range[0][0]=FLT_MAX;range[0][1]=FLT_MIN;
    range[1][0]=FLT_MAX;range[1][1]=FLT_MIN;
    range[2][0]=FLT_MAX;range[2][1]=FLT_MIN;
    for(int i=0;i<t.size();i++)
    {
         for(int j=0;j<3;j++)
         {
             if(t[i].range[j][0]<range[j][0])
                 range[j][0]=t[i].range[j][0];
             if(t[i].range[j][1]>range[j][1])
                 range[j][1]=t[i].range[j][1];
         }

    }
    cout<<"Range:\n";
    cout<<range[0][0]<<"\t"<<range[0][1]<<"\n";
    cout<<range[1][0]<<"\t"<<range[1][1]<<"\n";
    cout<<range[2][0]<<"\t"<<range[2][1]<<"\n";
    calcBands(10);
}

void StlFile::calcBands(int n)
{
    bandlimits.resize(n+1);
    for(int i=0;i<n+1;i++)
        bandlimits[i]=range[2][0]+i*(range[2][1]-range[2][0])/float(n);

    bands.resize(n);
    for(int b=0;b<n;b++)
    for(int i=0;i<(int)t.size();i++)
    {
        if(t[i].isInZ(bandlimits[b], bandlimits[b+1]))
            bands[b].push_back(i);
    }

    for(int b=0;b<n;b++)
    {

        cout<<"Band "<<b<<" "<<bandlimits[b]<<" "<<bandlimits[b+1]<<" :"<<bands[b].size()<<endl;
    }

}

int StlFile::findBand(const float z)
{
    for(int b=0;b<bands.size();b++)
    {

        if(bandlimits[b]<=z && bandlimits[b+1]>z)
            return b;
    }
    return  -1;
}
