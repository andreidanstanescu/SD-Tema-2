#include<iostream>
using namespace std;

class BTree;

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

    void Divide(int i,Node* y)   //O(t) --> foloseste metoda "cut-and-paste"
    {
        Node* z=new Node(t,y->is_leaf);
        z->nk=t-1;
        for(int j=0;j<t-1;j++)
            z->keys[j]=y->keys[j+t];

        if(!y->is_leaf)
        {for(int j=0;j<t-1;j++)
            z->fiu[j]=y->fiu[j+t];
        }

        y->nk=t-1;

        for(int j=nk-1;j>i;j--)
            keys[j+1]=keys[j];
        keys[i]=y->keys[t-1];

        for(int j=nk;j>i;j--)
            fiu[j+1]=fiu[j];
        fiu[i+1]=z;

        ++nk;
    }

    void Insert_neplin(int key)
    {
        //cout<<key<<' ';
        if(is_leaf)
        {
            int i;
            for(i=nk-1;i>=0 && key<keys[i];i--)
                keys[i+1]=keys[i];
            /*while(i>=0 && key<keys[i])
            {keys[i+1]=keys[i];
            i--;
            }*/
            keys[i+1]=key;
            nk++;
            //cout<<i;
        }
        else
        {
            //regula este sa inseram mereu intr-o frunza
            //vedem pe ce fiu al nodului curent mergem mai departe
            int i=0;
            while(key>keys[i])
                i++;
            //cout<<key<<' '<<fiu[i]->nk<<' ';
            //i--;
            if(fiu[i]->nk==2*t-1)
            {
                this->Divide(i,fiu[i]);
                if(key<keys[i])
                    fiu[i]->Insert_neplin(key);
                else
                    fiu[i+1]->Insert_neplin(key);
            }
            else
                fiu[i]->Insert_neplin(key);


        }

    }

    int find_pred(int key)
    {
        int i,ans=INT_MIN;
        for(i=0;i<nk;i++){
            if(keys[i]>=key)
                break;
            if(ans<keys[i])
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
            if(keys[i]<=key)
                break;
            if(ans>keys[i])
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
        int i=0;

        while(i<nk && keys[i]<x)
            ++i;

        while(i<nk)
        {
            if(!is_leaf)
                if(i>0 && keys[i-1]<=y)
                fiu[i]->afisare(x,y);
        if(keys[i]>=x && keys[i]<=y)
            {
                cout<<keys[i]<<' ';
            }
            ++i;

        }
        //are nk+1 fii
        if(!is_leaf)
            if(nk && keys[nk-1]<=y)
            fiu[nk]->afisare(x,y);

    }

    void Bsterg(int val){
    int i=0;
    while(i<nk && keys[i]<val)
        i++;
    //cout<<i<<' '<<keys[i-1]<<endl;
    if(i==nk || (i<nk && keys[i]!=val)) //handling Case 3
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
        if(fiu[i]->nk==t-1)
            taking(i);

        if(la_final==1 && i>nk)
            fiu[i-1]->Bsterg(val);
        else
            fiu[i]->Bsterg(val);

    }
    else
        if(i<nk && val==keys[i])
    {
        //cout<<i<<' '<<keys[i]<<endl;
        //cout<<is_leaf<<endl;
        if(is_leaf)
            delete_leaf(i);
        else
            delete_non_leaf(i);
    }


    }

    void delete_leaf(int index){

        for(int i=index;i<nk;i++)
            keys[i]=keys[i+1];
        --nk;

    }

    void delete_non_leaf(int index){

        int val2=keys[index];
        if(fiu[index]->nk>t-1)
        {
            //Case 2 a
            int x=find_pred(val2);
            keys[index]=x;
            fiu[index]->Bsterg(x);

        }
        else
            if(fiu[index+1]->nk>t-1)
        {
            //Case 2 b
            int x=find_suc(val2);
            keys[index]=x;
            fiu[index+1]->Bsterg(x);
        }
        else
        {
            //Case 2 c
            fuzionare(index);
            fiu[index]->Bsterg(val2);

        }

    }

    void taking(int index)
    {
        //........k..(fiu[index])...
        //Case 3 a:
        if(index>0 && fiu[index-1]->nk>=t)
            take_pred(index);
        else
            if(index<nk && fiu[index+1]->nk>=t)
            take_succ(index);
        else
        {
            //Case 3 b
            //fuzionez fiul curent cu precedentul sau urmatorul
            if(index<nk)
                fuzionare(index);
            else
                fuzionare(index-1);
        }


    }

    void take_pred(int index)
    {
        //mut toate cheile mai in dreapta(cu referintele catre fii) cu o unitate
        for(int i=fiu[index]->nk;i>0;i--)
            keys[i]=keys[i-1];
        if(!fiu[index]->is_leaf)
        {
            for(int i=fiu[index]->nk+1;i>0;i--)
            fiu[index]->fiu[i]=fiu[index]->fiu[i-1];

        }

        fiu[index]->keys[0]=keys[index-1];

        Node* pred=fiu[index-1];
        fiu[index]->fiu[0]=pred->fiu[pred->nk];

        fiu[index]->nk++;

        keys[index-1]=pred->keys[pred->nk];
        pred->nk--;

    }

    void take_succ(int index)
    {
        fiu[index]->keys[fiu[index]->nk]=keys[index];

        Node* next=fiu[index+1];

        if(!fiu[index]->is_leaf)
        {
            fiu[index]->fiu[(fiu[index]->nk)+1]=next->fiu[0];
        }

        keys[index]=next->keys[0];

        fiu[index]->nk++;

        //shiftez la stanga cu o unitate toate cheile din fratele de la dreapta
        for(int i=1;i<next->nk;i++)
            next->keys[i-1]=next->keys[i];

        if(!next->is_leaf)
        {
            for(int i=1;i<=next->nk;i++)
            next->fiu[i-1]=next->fiu[i];
        }
        next->nk--;

    }

    void fuzionare(int index)
    {
        Node* curr=fiu[index];
        Node* next=fiu[index+1];

        curr->keys[t-1]=keys[index-1];
        ++curr->nk;

        for(int i=0;i<next->nk;i++)
            curr->keys[i+t]=next->keys[i];
        if(!curr->is_leaf)
        {
            for(int i=0;i<=next->nk;i++)
            curr->fiu[i+t]=next->fiu[i];
        }

        for(int i=index;i<nk-1;++i)
            keys[i]=keys[i+1];

        for(int i=index+1;i<nk;++i)
            fiu[i]=fiu[i+1];

        --nk;
        curr->nk+=next->nk;
        delete next;

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
        //if(root)
        root->parcurgere();
    }

    void Insert(int key)  //worst-case O(th) ---> la inserare se adauga la o frunza si fiecare nod poate fi plin
    {
        if(root==NULL)
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
            root=s;
            if(key<s->keys[0])
                s->fiu[0]->Insert_neplin(key);
            else
                s->fiu[1]->Insert_neplin(key);
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
            if(root->is_leaf)
                root=nullptr;
            else
            {
            Node* aux=root;
            root=root->fiu[0];
            delete aux;
            }
        }
        root->Bsterg(key);
    }

    void afisare(int x,int y)
    {
        root->afisare(x,y);
    }


};

int main()
{
    BTree t(3);
    t.Insert(10);
    t.Insert(120);
    t.Insert(4);
    t.Insert(9);
    t.Insert(12);
    t.Insert(17);
    t.Insert(14);
    t.Insert(34);
    t.afisare(7,33);
    t.Bsterge(68);
    t.Bsterge(45);
    t.Bsterge(34);
    cout<<endl;
    t.parcurgere();
    //cout<<t.get_pred(10);
    //cout<<t.get_suc(80);

}
