#include <bits/stdc++.h>
using namespace std;

//*********************** DEFINE CLASS ********************

class neron;

class layer{
    public :
        layer(int num_neron,int n_layer);
        vector < neron > m_layer;
    private:
};

//*********************** CONECTION ***********************

struct conection{
    double weight=1.0f*rand()/RAND_MAX;
    double deltaweight=0;
};

//************************ NERON ***************************

class neron{
    public :
        neron(int n_layer);
        vector < conection > m_neron;
        double output;
        void calOutput(layer prelayer,int n_index);
        double getOutput();
        double gradient;
    private :
};

neron::neron(int n_layer){

    for(int i=0;i<n_layer;++i)
    {
        m_neron.push_back(conection());
    }
}

void neron::calOutput(layer prelayer,int n_index)
{   
    output=0;

    for(int i=0;i<prelayer.m_layer.size();++i)
    {   
        output+=prelayer.m_layer[i].getOutput()*prelayer.m_layer[i].m_neron[n_index].weight;
    }
}

double neron::getOutput()
{
    return tanh(output);
}

//************************ LAYER **************************

layer::layer(int num_neron,int n_layer){

    for(int i=0;i<=num_neron;++i)
    {
        m_layer.push_back(neron(n_layer));
    }

    m_layer.back().output=1;
}

//*************************** NET ******************************

class net{
    public :
        net(vector < int > &topo);
        vector < layer > m_net;
        void input(vector <double > &inputval);
        void result(vector < double > &resultval);
        void backpro(vector < double > &targetval);
    private :
};

net::net(vector < int > &topo){

    for(int cnt=0;cnt<topo.size();++cnt)
    {   
        int n_layer=cnt==topo.size()-1 ? 0 : topo[cnt+1];

        m_net.push_back(layer(topo[cnt],n_layer));
    }
}

void net::input(vector < double > &inputval)
{
    for(int i=0;i<inputval.size();++i)
    {
        m_net[0].m_layer[i].output=inputval[i];
    }
}

void net::result(vector < double > &resultval)
{   
    for(int i=1;i<m_net.size();++i)
    {
        for(int j=0;j<m_net[i].m_layer.size()-1;++j)
        m_net[i].m_layer[j].calOutput(m_net[i-1],j);
    }

    resultval.clear();

    for(int cnt=0;cnt<m_net.back().m_layer.size();++cnt)
    {   
        resultval.push_back(m_net.back().m_layer[cnt].getOutput());
    }
}

void net::backpro(vector < double > &targetval)
{
    for(int i=0;i<m_net.back().m_layer.size();++i)
    {
        m_net.back().m_layer[i].gradient=2*(m_net.back().m_layer[i].getOutput()-targetval[i])/m_net.back().m_layer.size();
    }

    for(int i=m_net.size()-2;i>=0;--i)
    { 
        for(int j=0;j<m_net[i].m_layer.size();++j)
        {   
            m_net[i].m_layer[j].gradient=0;

            for(int z=0;z<m_net[i].m_layer[j].m_neron.size();++z)
            {   
                //dF/dweight
                double val=m_net[i+1].m_layer[z].gradient;
                val*=1-m_net[i+1].m_layer[z].getOutput()*m_net[i+1].m_layer[z].getOutput();
                val*=m_net[i].m_layer[j].getOutput();
                val=val/2+m_net[i].m_layer[j].m_neron[z].deltaweight/2;
                m_net[i].m_layer[j].m_neron[z].deltaweight=val;
                m_net[i].m_layer[j].m_neron[z].weight-=m_net[i].m_layer[j].m_neron[z].deltaweight*0.1;

                //dF/dgetoutput
                val=m_net[i+1].m_layer[z].gradient;
                val*=1-m_net[i+1].m_layer[z].getOutput()*m_net[i+1].m_layer[z].getOutput();
                val*=m_net[i].m_layer[j].m_neron[z].weight;

                m_net[i].m_layer[j].gradient+=val;
            }
        }
    }
}

//*************************** MAIN *****************************

int main (){   

    vector < int > topo;
    topo.push_back(2);
    topo.push_back(4);
    topo.push_back(2);
    net mynet(topo);

    vector < double > inputval;
    vector <double> resultval;
    vector <double> targetval;
    
    for(int i=0;i<2000;++i)
    {
        int x=rand()%2;
        int y=rand()%2;

        inputval.clear();
        inputval.push_back(x);
        inputval.push_back(y);

        mynet.input(inputval);

        int z=x^y;
        
        targetval.clear();

        if(!z)
        targetval.push_back(1),targetval.push_back(0);
        else
        targetval.push_back(0),targetval.push_back(1);
        
        mynet.result(resultval);
        mynet.backpro(targetval);
    }
    
    int ans=0;

    for(int i=0;i<2000;++i)
    {
        int x=rand()%2;
        int y=rand()%2;

        inputval.clear();
        inputval.push_back(x);
        inputval.push_back(y);

        mynet.input(inputval);

        int z=x^y;
        
        mynet.result(resultval);

        int nerz;

        if(resultval[0]>resultval[1])
        nerz=0;
        else
        nerz=1;

        if(nerz==z)
        ans++;
    }
    
    cout << 1.0f*ans/2000;
}