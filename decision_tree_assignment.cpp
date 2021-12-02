#include <bits/stdc++.h>
using namespace std;
int n,m,numnode;
vector <string> name;
int feature_size[5],numf=0;
vector <vector <int> > train,valid,test;
vector <string> classes;
struct node
{
    int stop,depth;
    pair <int,int> split;
    string predict;
    node *left,*right;
};
struct tree
{
    node* root;
};
node *root;
double gini_base,entropy_base;
int maxdepth,minsize;
vector <double> important_score;
vector <string> split(string s,char c)
{
    string buff="";s+=c;
    vector <string> res;
    for (char x:s)
    {
        if (x==c)
        {
            if (buff.size()>0)
                res.push_back(buff);
            buff="";
        }
        else buff+=x;
    }
    return res;
}
void get_train_data(string input)
{
    train.clear();
    fstream inp;
    inp.open(input.c_str(),ios::in);
    string line;
    int index=0;
    while (getline(inp,line))
    {
        train.push_back({});
        vector <string> vs=split(line,',');
        if (numf==0) numf=vs.size()-1;
        assert(numf==vs.size()-1);
        if (vs.size()==0) break;
        int exist=-1;
        for (int i=0;i<classes.size();i++)
            if (classes[i]==vs[0]) exist=i;
        if (exist==-1)
        {
            classes.push_back(vs[0]);
            train.back().push_back(classes.size()-1);
        }
        else train.back().push_back(exist);
        for (int i=1;i<=numf;i++)
        {
            train.back().push_back(stoi(vs[i]));
            feature_size[i]=max(feature_size[i],train.back().back());
        }
    }
    n=train.size();
}
void get_valid_test(string input)
{
    valid.clear();
    fstream inp;
    inp.open(input.c_str(),ios::in);
    string line;
    int index=0;
    while (getline(inp,line))
    {
        vector <string> vs=split(line,',');
        assert(numf==vs.size()-1);
        if (vs.size()==0) break;
        valid.push_back({});
        for (int i=0;i<classes.size();i++)
            if (classes[i]==vs[0]) valid.back().push_back(i);
        for (int i=1;i<=numf;i++)
        {
            valid.back().push_back(stoi(vs[i]));
        }
    }

    m=valid.size();
}
void get_hidden_test(string input)
{
    test.clear();
    fstream inp;
    inp.open(input.c_str(),ios::in);
    string line;
    int index=0;
    while (getline(inp,line))
    {
        vector <string> vs=split(line,',');
        if (vs.size()==0) break;
        test.push_back({});
        test.back().push_back(0);
        for (int i=0;i<numf;i++)
        {
            test.back().push_back(stoi(vs[i]));
        }
    }
}
double entropy(vector <int> dataset)
{
    if (dataset.size()==0) return 1e6;
    vector <int> num;num.resize(classes.size(),0);
    for (int index:dataset)
    {
        num[train[index][0]]++;
    }
    double H=0;
    for (int j=0;j<classes.size();j++)
    {
        double p=(num[j]/(double) dataset.size());
        if (p==0) continue;
        H+=-p*log2(p);
    }
    return H;
}
double gini(vector <int> dataset)
{
    if (dataset.size()==0) return 1e6;
    vector <int> num;num.resize(classes.size(),0);
    for (int index:dataset)
    {
        num[train[index][0]]++;
    }
    double res=1;
    for (int j=0;j<classes.size();j++)
    {
        double p=(num[j]/(double) dataset.size());
        res-=p*p;
    }
    return res;
}
string major_label(vector <int> dataset)
{
    vector <int> num;num.resize(classes.size(),0);
    for (int index:dataset)
    {
        num[train[index][0]]++;
    }
    string res;int temp=-1;
    for (int j=0;j<classes.size();j++)
    {
        if (temp<num[j])
        {
            temp=num[j];
            res=classes[j];
        }
    }
    return res;
}
void split(node *T,vector <int> dataset,int max_depth,int min_size,string criterion)
{
    T->stop=0;
    if (dataset.size()<=min_size)
    {
        T->stop=1;
        T->predict=major_label(dataset);
        return;
    }
    if ((int) T->depth==max_depth)
    {
        T->stop=1;
        T->predict=major_label(dataset);
        return;
    }
    if (criterion=="gini")
        if (gini(dataset)<=gini_base)
        {
            T->stop=1;
            T->predict=major_label(dataset);
            return;
        }
    if (criterion=="entropy")
        if (entropy(dataset)<=entropy_base)
        {
            T->stop=1;
            T->predict=major_label(dataset);
            return;
        }

    double smin=1e9;pair <int,int> threshold={0,0};
    for (int f=1;f<=numf;f++)
    {
        for (int j=1;j<=feature_size[f]-1;j++)
        {
            vector <int> child1;
            vector <int> child2;
            for (int index:dataset)
                if (train[index][f]<=j) child1.push_back(index);
                else child2.push_back(index);
            if (child1.size()==0||child2.size()==0) continue;
            double s=0;
            if (criterion=="gini") s=gini(child1)*child1.size()+gini(child2)*child2.size();
            if (criterion=="entropy") s=entropy(child1)*child1.size()+entropy(child2)*child2.size();
            if (smin>s)
            {
                smin=s;
                threshold={f,j};
            }
        }
    }
    if (threshold.first==0)
    {
        T->stop=1;
        T->predict=major_label(dataset);
        return;
    }
    T->split=threshold;
    vector <int> child1;
    vector <int> child2;
    for (int index:dataset)
        if (train[index][threshold.first]<=threshold.second) child1.push_back(index);
        else child2.push_back(index);
    T->left=new(node);T->left->depth=T->depth+1;
    T->right=new(node);T->right->depth=T->depth+1;
    split(T->left,child1,max_depth,min_size,criterion);
    split(T->right,child2,max_depth,min_size,criterion);
}
void buildTree(vector <int> P,int max_depth,int min_size,string criterion)
{
    root=new(node);
    root->depth=0;
    split(root,P,max_depth,min_size,criterion);
}
string predict(vector <int> datapoint)
{
    node *T=root;
    while (1)
    {
        if (T->stop==1) return T->predict;
        pair <int,int> threshold=T->split;
        if (datapoint[threshold.first]<=threshold.second)
            T=T->left;
        else T=T->right;
    }
    return "unknown";
}
void export_node(node *T)
{

    for (int i=1;i<=T->depth;i++)
        cout<<"|   ";
    cout<<"|---";
    if (T->stop==1)
    {
        cout<<" class : "<<T->predict<<'\n';
    }
    else
    {
        cout<<" feature_"<<T->split.first<<" <= "<<(double) T->split.second+0.5<<'\n';
        export_node(T->left);
        export_node(T->right);
    }
}
void export_tree()
{
    freopen("tree.txt","w",stdout);
    export_node(root);
}
int get_valid_corrects()
{
    int corrects=0;
    for (int i=0;i<valid.size();i++)
    {
        if (classes[valid[i][0]]==predict(valid[i])) corrects++;
       // cout<<valid[i].size()<<'\n';
    }
    return corrects;
}
int get_train_corrects()
{
    int corrects=0;
    for (int i=0;i<train.size();i++)
        if (classes[train[i][0]]==predict(train[i])) corrects++;
    return corrects;
}
void get_confusion_matrix(vector <vector<int> > dataset)
{
    cout<<"confusion matrix:\n";
    int num[100][100];memset(num,0,sizeof(num));
    for (vector <int> row: dataset)
    {
        for (int i=0;i<classes.size();i++)
            if (predict(row)==classes[i]) num[row[0]][i]++;
    }
    for (int i=0;i<classes.size();i++)
    {
        for (int j=0;j<classes.size();j++)
            cout<<num[i][j]<<' ';
        cout<<'\n';
    }
}
void train_decision_tree_classcifier()
{
    cout<<"number of data rows: "<<n<<'\n';
    cout<<"number of features: "<<numf<<endl;
    vector <int> P;
    for (int i=0;i<n;i++)
        P.push_back(i);
    int res=0;
    int save_maxdepth,save_minsize;
    double save_base;double max_rate=0;

    ofstream details;
    details.open("result_details.txt");
    vector <double> train_details, valid_details;
    for (int maxdepth=3;maxdepth<=7;maxdepth++)
    {
        train_details.clear();
        valid_details.clear();
        for (int minsize=0;minsize<=60;minsize++)
            //for (double base=0;base<0.2;base+=0.01)
            {
                buildTree(P,maxdepth,minsize,"gini");
                if (res<get_valid_corrects()){
                    res=get_valid_corrects();
                    save_maxdepth=maxdepth;
                    save_minsize=minsize;
                }
                train_details.push_back((double) get_train_corrects()/train.size());
                valid_details.push_back((double) get_valid_corrects()/valid.size());
            }
        details<<"max depth = "<<maxdepth<<'\n';
        for (int i=0;i<=60;i++)
            details<<fixed<<setprecision(5)<<train_details[i]<<',';
        details<<'\n';
        for (int i=0;i<=60;i++)
            details<<fixed<<setprecision(5)<<valid_details[i]<<',';
        details<<'\n';
    }
    details.close();

    buildTree(P,save_maxdepth,save_minsize,"gini");
    cerr<<"best validation accuracy rate:  "<<(double) res/valid.size()<<"    - train: "<<(double) get_train_corrects()/train.size()<<endl;
    cerr<<"best max_depth and min_size: "<<save_maxdepth<<' '<<save_minsize<<'\n';
    get_confusion_matrix(valid);

}
void test_and_export_result(string output)
{
    ofstream out;
    out.open(output.c_str());
    for (int i=0;i<test.size();i++)
    {
        out<<predict(test[i])<<'\n';
    }
}
int main()
{
    get_train_data("train.txt");
    get_valid_test("valid.txt");
    get_hidden_test("private_test.txt");
    train_decision_tree_classcifier();
    export_tree();

    test_and_export_result("test_result.txt");


}


