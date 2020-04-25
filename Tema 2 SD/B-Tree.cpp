#include<bits/stdc++.h>
#include<fstream>
using namespace std;

class BTree;

ifstream fin("abce.in");
ofstream fout("abce.out");

class Node{
private:
    Node** fiu;
    int nk; //numarul de chei (n(x))
    int t; //gradul minim al B-arborelui
    int* keys;
    bool is_leaf;
public:
    Node(int _t,bool _is_leaf)
    {
        t=_t;
        is_leaf=_is_leaf;

        //aloc memorie vectorului de chei si de fii
        nk=0;
        keys=new int[2*t-1];

        //aloc pentru fii
        fiu=new Node*[2*t];

        //am alocat nr maxim posibil in functie de t (oricum ii eliberez la final)

    }

    void parcurgere()
    {
        for(int i=0;i<nk;i++)
        {
            if(!is_leaf)
                fiu[i]->parcurgere();
            cout<<keys[i]<<' ';
        }
        //are nk+1 fii
        if(!is_leaf)
            fiu[nk]->parcurgere();
    }

    //dupa Cormen
    Node* cauta(int key)            //O(h) adica O(logn), deoarece h<=logt((n+1)/2)
    {
        int i=0;
        while(i<nk && key>keys[i])
            i++;
        if(i<nk && key==keys[i])
            return this;
        if(is_leaf)
            return nullptr;
        return fiu[i]->cauta(key);
    }

    bool find(int key)
    {
        int i=0;
        while(i<nk && key>keys[i])
            i++;
        if(i<nk && key==keys[i])
            return 1;
        if(is_leaf)
            return 0;
        return fiu[i]->find(key);
    }

    void Divide(int i,Node* y)   //O(t) --> foloseste metoda "cut-and-paste"
    {
    Node *z = new Node(y->t, y->is_leaf);
    z->nk = t - 1;

    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];

    if (y->is_leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->fiu[j] = y->fiu[j+t];
    }

    y->nk = t - 1;

    for (int j = nk; j >= i+1; j--)
        fiu[j+1] = fiu[j];

    fiu[i+1] = z;


    for (int j = nk-1; j >= i; j--)
        keys[j+1] = keys[j];


    keys[i] = y->keys[t-1];

    ++nk;

    }

    void Insert_neplin(int key)
    {
        int i = nk-1;

    if (is_leaf )
    {

        while (i >= 0 && keys[i] > key)
        {
            keys[i+1] = keys[i];
            i--;
        }

        keys[i+1] = key;
        nk = nk+1;
    }
    else
    {
        while (i >= 0 && keys[i] > key)
            i--;

        if (fiu[i+1]->nk == 2*t-1)
        {
            Divide(i+1, fiu[i+1]);

            if (keys[i+1] < key)
                i++;
        }
        fiu[i+1]->Insert_neplin(key);
    }

    }

    int find_pred(int key)
    {
        int i,ans=INT_MIN;
        for(i=0;i<nk;i++){
            if(keys[i]>key)
                break;
            if(ans<=keys[i])
            {
                //cout<<keys[i]<<' ';
                ans=keys[i];
                //ans=max(ans,fiu[i]->find_pred(key));

            }
        }
        if(!is_leaf)
            ans=max(ans,fiu[i]->find_pred(key));
        return ans;
    }

    int find_suc(int key)
    {
        int i,ans=INT_MAX;
        for(i=nk-1;i>=0;--i){
            if(keys[i]<key)
                break;
            if(ans>=keys[i])
            {
                //cout<<keys[i]<<' ';
                ans=keys[i];
                //ans=max(ans,fiu[i]->find_pred(key));

            }
        }
        if(!is_leaf)
            ans=min(ans,fiu[i+1]->find_suc(key));
        return ans;
    }

    void afisare(int x,int y)
    {

        for(int i=0;i<nk;++i)
        {
            if(!is_leaf)
                //if(i>0 && keys[i-1]<=y)
                fiu[i]->afisare(x,y);
        if(keys[i]>=x && keys[i]<=y)
            {
                fout<<keys[i]<<' ';
            }

        }
        //are nk+1 fii
        if(!is_leaf)

            fiu[nk]->afisare(x,y);

    }

    int getPred(int idx)
    {
    Node *cur=fiu[idx];
    while (!cur->is_leaf)
        cur = cur->fiu[cur->nk];
    return cur->keys[cur->nk-1];
    }

    int getSucc(int idx)
    {
    Node *cur = fiu[idx+1];
    while (!cur->is_leaf)
        cur = cur->fiu[0];

    return cur->keys[0];
    }

    void Bsterg(int val){
    int i=0;
    while(i<nk && keys[i]<val)
        i++;
    //cout<<i<<' '<<keys[i-1]<<endl;

    if(i<nk && val==keys[i])
    {
        //cout<<i<<' '<<keys[i]<<endl;
        //cout<<is_leaf<<endl;
        if(is_leaf)
            delete_leaf(i);
        else
            delete_non_leaf(i);
    }
    else //handling Case 3
    {
        if(is_leaf)
        {
            //cout<<"Nu avem "<<val<<endl;
            return;
        }
        bool la_final=0;
        if(i==nk)
            la_final=1;
        //asa cum e prezentat in Cormen:
        //daca fiu[i]->nk=t-1, asigur cardinalitatea de t (in urma posibilelor stergeri conform cazului 2, ma asigur
        //ca, chiar daca sterg in Case 2 C, tot am minim t chei in nodul de unde sterg
        if(fiu[i]->nk<t)
            taking(i);

        if(la_final==1 && i>nk)
            fiu[i-1]->Bsterg(val);
        else
            fiu[i]->Bsterg(val);

    }
    return;
    }

    void delete_leaf(int index){

        for(int i=index+1;i<nk;i++)
            keys[i-1]=keys[i];
        --nk;
        return;
    }

    void delete_non_leaf(int index){

        int val2=keys[index];
        if(fiu[index]->nk>=t)
        {
            //Case 2 a
            int x=getPred(index);
            keys[index]=x;
            fiu[index]->Bsterg(x);

        }
        else
            if(fiu[index+1]->nk>=t)
        {
            //Case 2 b
            int x=getSucc(index);
            keys[index]=x;
            fiu[index+1]->Bsterg(x);
        }
        else
        {
            //Case 2 c
            fuzionare(index);
            fiu[index]->Bsterg(val2);

        }
        return;

    }

    void taking(int index)
    {
        //........k..(fiu[index])...
        //Case 3 a:
        if(index!=0 && fiu[index-1]->nk>=t)
            take_pred(index);
        else
            if(index!=nk && fiu[index+1]->nk>=t)
            take_succ(index);
        else
        {
            //Case 3 b
            //fuzionez fiul curent cu precedentul sau urmatorul
            if(index!=nk)
                fuzionare(index);
            else
                fuzionare(index-1);
        }
        return;

    }

    void take_pred(int idx)
    {
    //mut toate cheile mai in dreapta(cu referintele catre fii) cu o unitate
    Node *child=fiu[idx];
    Node *sibling=fiu[idx-1];

    for (int i=child->nk-1; i>=0; --i)
        child->keys[i+1] = child->keys[i];

    if (!child->is_leaf)
    {
        for(int i=child->nk; i>=0; --i)
            child->fiu[i+1] = child->fiu[i];
    }

    child->keys[0] = keys[idx-1];

    if(!child->is_leaf)
        child->fiu[0] = sibling->fiu[sibling->nk];

    keys[idx-1] = sibling->keys[sibling->nk-1];

    child->nk += 1;
    sibling->nk -= 1;
    return;
    }

    void take_succ(int idx)
    {
    Node *child=fiu[idx];
    Node *sibling=fiu[idx+1];

    child->keys[(child->nk)] = keys[idx];

    if (!(child->is_leaf))
        child->fiu[(child->nk)+1] = sibling->fiu[0];

    keys[idx] = sibling->keys[0];

    for (int i=1; i<sibling->nk; ++i)
        sibling->keys[i-1] = sibling->keys[i];

    if (!sibling->is_leaf)
    {
        for(int i=1; i<=sibling->nk; ++i)
            sibling->fiu[i-1] = sibling->fiu[i];
    }

    child->nk += 1;
    sibling->nk -= 1;

    return;

    }

    void fuzionare(int idx)
    {
    Node *child = fiu[idx];
    Node *sibling = fiu[idx+1];

    child->keys[t-1] = keys[idx];

    for (int i=0; i<sibling->nk; ++i)
        child->keys[i+t] = sibling->keys[i];

    if (!child->is_leaf)
    {
        for(int i=0; i<=sibling->nk; ++i)
            child->fiu[i+t] = sibling->fiu[i];
    }

    for (int i=idx+1; i<nk; ++i)
        keys[i-1] = keys[i];

    for (int i=idx+2; i<=nk; ++i)
        fiu[i-1] = fiu[i];

    child->nk += sibling->nk+1;
    nk--;

    delete(sibling);
    return;

    }

    //inserarea o voi face din clasa BTree
friend class BTree;

};


class BTree{
private:
    Node* root;
    int t;
public:
    BTree(int _t)
    {
        t=_t;
        root=nullptr;
    }

    Node* cauta(int key)
    {
        return root->cauta(key);
    }

    void parcurgere()
    {
        if(root)
        root->parcurgere();
    }

    void Insert(int key)  //worst-case O(th) ---> la inserare se adauga la o frunza si fiecare nod poate fi plin
    {
        if(root==nullptr)
        {
            root=new Node(t,1);
            //cout<<key;
            root->keys[0]=key;
            root->nk=1;
        }
        else
            if(root->nk==2*t-1)
        {
            //cout<<key<<' ';
            Node* s=new Node(t,0);
            s->fiu[0]=root;
            s->Divide(0,root);
            int i = 0;
            if (s->keys[0] < key)
                i++;
            s->fiu[i]->Insert_neplin(key);

            root = s;
        }
            else
            {
                root->Insert_neplin(key);
                //cout<<key<<' ';
            }

    }

    int get_pred(int key)
    {
        return root->find_pred(key);
    }

    int get_suc(int key)
    {
        return root->find_suc(key);
    }

    void Bsterge(int key)
    {
        if(!root->nk)
        {
            return;
        }

        root->Bsterg(key);

        if (root->nk==0)
        {
            Node *temp = root;
            if (root->is_leaf)
                root = nullptr;
            else
                root = root->fiu[0];
            delete temp;
        }
        return;
    }

    void afisare(int x,int y)
    {
        root->afisare(x,y);
    }

    bool find(int key)
    {
        if(root==nullptr)
            return 0;
        return root->find(key);
    }


};

int main()
{
    int q,m,x,y;
    BTree t(3);
    fin>>q;
    while(q--)
    {
        fin>>m;
        if(m==1)
        {
            fin>>x;
            t.Insert(x);

        }
        if(m==2)
        {
            fin>>x;
            t.Bsterge(x);
        }
        if(m==3){
            fin>>x;
            fout<<t.find(x)<<'\n';}
        if(m==4)
        {
            fin>>x;
            fout<<t.get_pred(x)<<'\n';
        }
        if(m==5)
        {
            fin>>x;
            fout<<t.get_suc(x)<<'\n';
        }
        if(m==6)
        {
            fin>>x>>y;
            t.afisare(x,y);
            //t.parcurgere();
            fout<<'\n';
        }

    }
    fin.close();
    fout.close();
    /*t.Insert(10);
    t.Insert(120);
    t.Insert(4);
    t.Insert(9);
    t.Insert(12);
    t.Insert(17);
    t.Insert(14);
    t.Insert(34);
    //t.afisare(7,33);
    t.Bsterge(68);
    t.Bsterge(45);
    t.Bsterge(4);
    t.Bsterge(17);
    t.Bsterge(9);
    t.Bsterge(10);
    t.Bsterge(120);
    t.Bsterge(8);
    t.Bsterge(5);

    t.parcurgere();*/
    //cout<<t.get_pred(10);
    //cout<<t.get_suc(80);

}
