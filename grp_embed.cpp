#include<bits/stdc++.h>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node* prev;

    Node(int data) : data(data), next(nullptr), prev(nullptr) {}
};
class LinkedList {
public:
    Node*head;
    Node*tail;
    LinkedList() : head(nullptr), tail(nullptr) {}
    // Function to insert a new node at the end
    void insert(int data,unordered_map<int,Node*>&mp) {
        Node* newNode = new Node(data);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        mp[data]=newNode;
    }
    // Function to insert a new node before a given node
    void insertBefore(Node* targetNode, int data,unordered_map<int,Node*>&mp) {
        if (!targetNode) {
            cout << "Given target node is null." << endl;
            return;
        }
        Node* newNode = new Node(data);
        newNode->next = targetNode;
        newNode->prev = targetNode->prev;
        if (targetNode->prev) {
            targetNode->prev->next = newNode;
        } else {
            head = newNode;
        }
        targetNode->prev = newNode;
        mp[data]=newNode;
    }

    // Function to insert a new node after a given node
    void insertAfter(Node* targetNode, int data,unordered_map<int,Node*>&mp) {
        if (!targetNode) {
            cout << "Given target node is null." << endl;
            return;
        }
        Node* newNode = new Node(data);
        newNode->prev = targetNode;
        newNode->next = targetNode->next;
        if (targetNode->next) {
            targetNode->next->prev = newNode;
        } else {
            tail = newNode;
        }
        targetNode->next = newNode;
        mp[data]=newNode;
    }
    // Function to print the list
    void display() const {
        Node* current = head;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
    void get_pos(vector<int>&pos)
    {
        Node*temp=head;
        int i=0;
        while(temp)
        {
            pos[temp->data]=++i;
            temp=temp->next;
        }
    }
    void convert_linkedlist_to_array(vector<int>&st_order)
    {
        while(head)
        {
            st_order.push_back(head->data);
            Node*temp=head;
            head=head->next;
            delete(temp);
        }
    }
};

void read_graph(string txt,vector<vector<int>> &adj)//this function is to read graph from text file
{
    ifstream file;

    file.open(txt);

    string line;

    while(getline(file,line))//getting the value of every line in the form string
    {                                   
        int k=line.size();
        int j=0;
        int n=0;
        int m=0;
        //converting numbers from string to integer and store them in adjacency list
        while(line[j]!=' ')
        {
            n=n*10+(line[j]-'0');
            j++;
        }
        while(line[j]==' ')
        {
            j++;
        }
        while(j<k && line[j]>=48 && line[j]<=57)
        {
            m=m*10+(line[j]-'0');
            j++;
        }
        adj[n].push_back(m);
        adj[m].push_back(n);
    }
    file.close();
}
// bool isadjacent(vector<vector<int>>&adj,int n,int m) // to know whether the two vertices for given for the st ordering is adjacent or not
// {
//     for(int i:adj[n])
//     {
//         if(i==m)
//         {
//             return true;
//         }
//     }
//     return false;
// }
void DFS_st(vector<vector<int>>& adj, int vertex, vector<bool>& vis,vector<int>&sign ,vector<int>&low,LinkedList &st,vector<int>&parent,int &s,int &t,unordered_map<int,Node*>&mp) {
    // Mark the current node as visited
    vis[vertex] = true;
    if ((vertex!=s) && (vertex!=t))
    { 
          if(sign[low[vertex]]==1)
            {
                    st.insertAfter(mp[parent[vertex]],vertex,mp);
                    sign[parent[vertex]]=-1;
            }
          if(sign[low[vertex]]==-1)
            {
                    st.insertBefore(mp[parent[vertex]],vertex,mp);
                    sign[parent[vertex]]=1;
            }
    }
    // Recur for all the vertices adjacent to this vertex
    for (int v : adj[vertex]) {
        if (!vis[v]) {
            DFS_st(adj, v, vis,sign,low,st,parent,s,t,mp);  
        }
        
    }
}
void DFSUtil(int v, vector<vector<int>> &adj, vector<bool>& visited, vector<int>& parent, vector<int>& disc, vector<int>& finish, int& time,vector<int> &low) {
    // Mark the current node as visited and set the discovery time
    visited[v] = true;
    disc[v] = ++time;
    low[v]=v;

    for (int i : adj[v]) {
        if (!visited[i]) {
            parent[i] = v;
            DFSUtil(i, adj, visited, parent, disc, finish, time,low);
            low[v]=min(low[i],low[v]);
        }
        if(visited[i] && low[i]!=-1)
        {
            if(disc[i]<disc[low[v]])
            {
                low[v]=i;
            }
        }
    }

    // All adjacent vertices are processed, so set the finish time of this vertex
    finish[v] = ++time;
}
void sort_in_neighbour(vector<vector<int>> &in_neighbour,vector<int> &pos)
{
   auto comp = [&](int a, int b) {
        return pos[a] < pos[b];  // Sort based on position in the pos array
    };
    for(vector<int>&v:in_neighbour)
    {
        sort(v.begin(),v.end(),comp);
    }
}

// Function to perform DFS traversals to get st_ordering and perform embedding for the given graph 
void embeding(int V, vector<vector<int>> &adj,int &s,int &t) {

    vector<bool> visited(V, false);
    vector<int> parent(V, 0); // Parent of each vertex
    vector<int> disc(V, -1); // Discovery time of each vertex
    vector<int> finish(V, -1); // Finish time of each vertex
    vector<int> low(V,-1);// hos low value for each vertex
    int time = 0;

    DFSUtil(s,adj,visited,parent,disc,finish,time,low);//dfs from s to get low,disc,parent

    vector<bool>vis(V,false);
    LinkedList st;
    vector<int>sign(V,-1);
    unordered_map<int,Node*>mp;
    
    st.insert(s,mp);
    st.insert(t,mp);

    DFS_st(adj,s,vis,sign,low,st,parent,s,t,mp);//to compute st_ordering for given graph

    vector<int>pos(V);
    st.get_pos(pos);//has position of vertices according to st_ordering

    vector<int>st_order;
    st.convert_linkedlist_to_array(st_order);//converted linkedlist to array

    vector<int>in(V,0);//has indegree for the vertex based on st ordering
    vector<int>out(V,0);//has outdegree for the vertex based on st ordering
    vector<vector<int>>in_neighbour(V);//has inneighbours for all vertex
    vector<vector<int>>out_neighbours(V);
    //this for loop is for getting in and outdegree for vertex and inneighbours too
    for(int p=0;p<V;p++)
    {
        int v=st_order[p];
        for(int w:adj[v])
        {
            if(pos[w]<pos[v])
            {
                in[v]++;
                in_neighbour[v].push_back(w);
            }
            else{
                out[v]++;
                out_neighbours[v].push_back(w);
            }
        }
    }
    sort_in_neighbour(in_neighbour,pos);

    map<pair<int,int>,int>edge;//stores x coordinates where two vertices are getting connected
    vector<vector<int>>embed(V);//has columns available for that particular vertex

    int n=st_order[0];
    if (out[n]==2)
    {
        embed[n]={0,1};
    }
    else if(out[n]==3)
    {
        embed[n]={0,-1,1};
    }
    else if(out[n]==4)
    {
        embed[n]={0,-1,1,2};
    }

    int m;
    vector<int>set(V);//has x coor for the particular vertex
    vector<int>y_coor(V);
    int u=0;
    y_coor[n]=u++;
    set.push_back(0);
    for(int i=1;i<V;i++)
    {
        n=st_order[i];
        y_coor[n]=u++;
        if(in[n]==1)
        {
            int w=in_neighbour[n][0];
            m=embed[w][0];//(first element from the inneighbour list);we can call as x_i
            set[n]=m;
            edge[{n,w}]=m;
            embed[w].erase(embed[w].begin());//remove that element from that inneighbour list

            if(out[n]==3)
            {
                embed[n]={m,m-1,m+1};//make columns
                for(int j=0;j<i;j++)
                {
                    if(set[st_order[j]]<m)//x_j < x_i
                    {
                        --set[st_order[j]];//-- x_j 
                    }
                    else if(set[st_order[j]]>m)//x_j > x_i
                    {
                        ++set[st_order[j]];//++ x_j
                    }
                    for(int k=0;k<embed[st_order[j]].size();k++)
                    {
                        if(embed[st_order[j]][k]<m)//move all the columns to the left by 1 unit which are on the left to that element(x_i)
                        {
                            --embed[st_order[j]][k];
                        }
                        else if(embed[st_order[j]][k]>m)//move all the columns to the right by 1 unit which are on the right to that element(x_i);
                        {
                            ++embed[st_order[j]][k];
                        }
                    }
                    for(int i:in_neighbour[st_order[j]])//for setting up edge x_coor
                    {
                        if(edge[{st_order[j],i}]<m)
                        {
                            --edge[{st_order[j],i}];
                        }
                        else if(edge[{st_order[j],i}]>m)
                        {
                            ++edge[{st_order[j],i}];
                        }
                    }
                }
            }
            else if(out[n]==2)
            {
                embed[n]={m,m-1};
                for(int j=0;j<i;j++)
                {
                    if(set[st_order[j]]<m)//x_j < x_i
                    {
                        --set[st_order[j]];//--x_j
                    }
                    for(int k=0;k<embed[st_order[j]].size();k++)
                    {
                        if(embed[st_order[j]][k]<m)
                        {
                            --embed[st_order[j]][k];
                        }
                    }
                    for(int i:in_neighbour[st_order[j]])//for setting up edge x_coor
                    {
                        if(edge[{st_order[j],i}]<m)
                        {
                            --edge[{st_order[j],i}];
                        }
                    }
                }
            }
            else{
                embed[n]={m};
            }

        }
        else if(in[n]==2)
        {
            int w_1=in_neighbour[n][0];
            int w_2=in_neighbour[n][1];
            int w=pos[w_1]<pos[w_2]?w_2:w_1;
            int q=pos[w_1]<pos[w_2]?w_1:w_2;
            m=embed[w][0];//(m is first element in most recent inneighbours list)(x_i);
            int x=embed[q][0];//(x is first element in last inneighbours list)
            set[n]=m;
            //remove all the inneighbours first element from their lists
            edge[{n,w_1}]=embed[w_1][0];
            edge[{n,w_2}]=embed[w_2][0];
            embed[w_1].erase(embed[w_1].begin());
            embed[w_2].erase(embed[w_2].begin());

            if(out[n]==1)
            {
                embed[n]={m};
            }
            else
            {
                if(m>x)//(if m is greater than x then make column on the right side and shift other coloumns by 1 unit on the right which is in right side)
                {
                    embed[n]={m,m+1};
                    for(int j=0;j<i;j++)
                    {
                        if(set[st_order[j]]>m)//x_j > x_i
                        {
                            ++set[st_order[j]];//++ x_j
                        }
                        for(int k=0;k<embed[st_order[j]].size();k++)
                        {
                            if(embed[st_order[j]][k]>m)
                            {
                                ++embed[st_order[j]][k];
                            }
                        }
                        for(int i:in_neighbour[st_order[j]])//for setting up edge x_coor
                        {
                            if(edge[{st_order[j],i}]>m)
                            {
                                ++edge[{st_order[j],i}];
                            }
                        }
                    }
                }
                else//(else make column on the left side and shift other coloumns by 1 unit on the left which is in left side)
                {
                    embed[n]={m,m-1};
                    for(int j=0;j<i;j++)
                    {
                        if(set[st_order[j]]<m)//x_j < x_i
                        {
                            --set[st_order[j]];//--x_j
                        }
                        for(int k=0;k<embed[st_order[j]].size();k++)
                        {
                            if(embed[st_order[j]][k]<m)
                            {
                                --embed[st_order[j]][k];
                            }
                        }
                        for(int i:in_neighbour[st_order[j]])//for setting up edge x_coor
                        {
                            if(edge[{st_order[j],i}]<m)
                            {
                                --edge[{st_order[j],i}];
                            }
                        }
                    }
                }
            }
            
        }
        else if(in[n]==3)
        {
            int w_1=in_neighbour[n][0];
            int w_2=in_neighbour[n][1];
            int w_3=in_neighbour[n][2];
            int w=pos[w_1]<pos[w_2]?w_2:w_1;
            w=pos[w]<pos[w_3]?w_3:w;
            m=embed[w][0];//(m is first element in most recent inneighbours list)(x_i);
            set[n]=m;
            //remove all inneighbours first element from their list
            edge[{n,w_1}]=embed[w_1][0];
            edge[{n,w_2}]=embed[w_2][0];
            edge[{n,w_3}]=embed[w_3][0];

            embed[w_1].erase(embed[w_1].begin());
            embed[w_2].erase(embed[w_2].begin());
            embed[w_3].erase(embed[w_3].begin());
            if(out[n]==1)
            {
                embed[n]={m};
            }
        }
        else if(in[n]==4)
        {
            int w_1=in_neighbour[n][0];
            int w_2=in_neighbour[n][1];
            int w_3=in_neighbour[n][2];
            int w_4=in_neighbour[n][3];
            int w=pos[w_1]<pos[w_2]?w_2:w_1;
            w=pos[w_3]<pos[w]?w:w_3;
            w=pos[w_4]<pos[w]?w:w_4;
            m=embed[w][0];//(m is first element in most recent inneighbours list)(x_i);
            set[n]=m;

            edge[{n,w_1}]=embed[w_1][0];
            edge[{n,w_2}]=embed[w_2][0];
            edge[{n,w_3}]=embed[w_3][0];
            edge[{n,w_4}]=embed[w_4][0];

            //remove all inneighbours first element from their list
            embed[w_1].erase(embed[w_1].begin());
            embed[w_2].erase(embed[w_2].begin());
            embed[w_3].erase(embed[w_3].begin());
            embed[w_4].erase(embed[w_4].begin());

            embed[n]={};
        }

    }
    
    //this loop is for printing x_coor of vertex and columns available for the particular vertex
    for(int i=0;i<V;i++)
    {
        cout<<"x coor and y coor for "<<st_order[i]<<" : "<<set[st_order[i]]<<" and "<<y_coor[i];
        cout<<"col available "<<st_order[i]<<":";
        for(int x:embed[st_order[i]])
        {
            cout<<x<<" ";
        }
        cout<<endl;
    }
    for(auto i=edge.begin();i!=edge.end();i++)
    {
        cout<<"edges got connected for vertices "<<i->first.first<<" and "<<i->first.second<<" is "<<i->second<<endl;
    }

    ofstream file1;
    ofstream file2;
    file1.open("node.txt");//node details will be in node.txt
    file2.open("edge.txt");//edge details will be in edge.txt
    int k=0;
    int j=0;
    int i=0;
    int st_element1=0;
    for(int i=1;i<V;i++)
    {
        int v=st_order[i];
        if(in[v]==1)
        {
            int n_1=in_neighbour[v][0];

            if(n_1==st_order[0])
            {
                st_element1++;
            }

            if(edge[{v,n_1}]==set[v] && edge[{v,n_1}]==set[n_1])
            {
                int source_1=k;
                int source_2=k+1;
                file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";
            }
            else{
                if(st_element1==4)
                {
                    int source_1=k;
                    int source_2=k+1;
                    int source_3=k+2;
                    int source_4=k+3;
                    file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string((y_coor[n_1]-1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_1}]*100)+","+to_string((y_coor[n_1]-1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                    file2<<to_string(j++)+","+to_string(source_3)+","+to_string(source_1)+".\n";
                    file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";
                    file2<<to_string(j++)+","+to_string(source_2)+","+to_string(source_4)+".\n";
                }
                else
                {
                    int source_1=k;
                    int source_2=k+1;
                    int source_3=k+2;
                    file1<<to_string(k++)+","+to_string(edge[{v,n_1}]*100)+","+to_string(y_coor[n_1]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                    file2<<to_string(j++)+","+to_string(source_2)+","+to_string(source_1)+".\n";
                    file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_3)+".\n";
                }
            }

        }
        if(in[v]==2)
        {
            int n_1=in_neighbour[v][1];
            int n_2=in_neighbour[v][0];

            if(n_2==st_order[0])
            {
                st_element1++;
            }

            if(edge[{v,n_1}]==set[v] && edge[{v,n_1}]==set[n_1])
            {
                int source_1=k;
                int source_2=k+1;
                file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";

                if(st_element1==4)
                {
                    int source_3=k;
                    int source_4=k+1;
                    int source_5=k+2;
                    int source_6=k+3;
                    int source_7=k+4;
                    file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string((y_coor[n_2]-1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string((y_coor[n_2]-1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                    file2<<to_string(j++)+","+to_string(source_3)+","+to_string(source_6)+".\n";
                    file2<<to_string(j++)+","+to_string(source_3)+","+to_string(source_4)+".\n";
                    file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_5)+".\n";
                    file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_7)+".\n";
                }
                else{
                        int source_3=k;
                        int source_4=k+1;
                        int source_5=k+2;
                        int source_6=k+3;
                        file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[n_2]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_3)+","+to_string(source_5)+".\n";
                        file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_3)+".\n";
                        file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_6)+".\n";
                }
            }
            else{
                int source_1=k;
                int source_2=k+1;
                int source_3=k+2;
                file1<<to_string(k++)+","+to_string(edge[{v,n_1}]*100)+","+to_string(y_coor[n_1]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_3)+".\n";

                if(st_element1==4)
                {
                    int source_4=k;
                    int source_5=k+1;
                    int source_6=k+2;
                    int source_7=k+3;
                    int source_8=k+4;
                    file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string((y_coor[n_2]-1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string((y_coor[n_2]-1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                    file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_4)+".\n";
                    file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_5)+".\n";
                    file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_6)+".\n";
                    file2<<to_string(j++)+","+to_string(source_6)+","+to_string(source_8)+".\n";
                }
                else{
                        int source_4=k;
                        int source_5=k+1;
                        int source_6=k+2;
                        int source_7=k+3;
                        file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[n_2]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_6)+","+to_string(source_4)+".\n";
                        file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_5)+".\n";
                        file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_7)+".\n";
                }                  
            }
        }
        if(in[v]==3)
        {
            int n_1=in_neighbour[v][2];
            int n_2=in_neighbour[v][1];
            int n_3=in_neighbour[v][0];

            if(n_3==st_order[0])
            {
                st_element1++;
            }

            if(edge[{v,n_1}]==set[v] && edge[{v,n_1}]==set[n_1])
            {
                int source_1=k;
                int source_2=k+1;
                file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";

                int source_3=k;
                int source_4=k+1;
                int source_5=k+2;
                int source_6=k+3;

                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[n_2]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_3)+".\n";
                file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_3)+".\n";
                file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_6)+".\n";
                if((edge[{v,n_2}]<set[v] && edge[{v,n_3}]<set[v])||(edge[{v,n_2}]>set[v] && edge[{v,n_3}]>set[v]))
                {
                    if(st_element1==4)
                    {
                        int source_7=k;
                        int source_8=k+1;
                        int source_9=k+2;
                        int source_10=k+3;
                        int source_11=k+4;
                        int source_12=k+5;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_11)+".\n";
                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_7)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                        file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_9)+".\n";
                        file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_12)+".\n";
                    }
                    else{
                        int source_7=k;
                        int source_8=k+1;
                        int source_9=k+2;
                        int source_10=k+3;
                        int source_11=k+4;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_10)+".\n";
                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_7)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_11)+".\n";
                    }
                }
                else
                {
                    if(st_element1==4)
                    {
                        int source_7=k;
                        int source_8=k+1;
                        int source_9=k+2;
                        int source_10=k+3;
                        int source_11=k+4;

                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_10)+".\n";
                        file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_8)+".\n";
                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_9)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_11)+".\n";
                    }
                    else
                    {
                        int source_7=k;
                        int source_8=k+1;
                        int source_9=k+2;
                        int source_10=k+3;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_7)+".\n";
                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_7)+".\n";
                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_10)+".\n";
                    }
                }
            }
            else{
                int source_1=k;
                int source_2=k+1;
                int source_3=k+2;

                file1<<to_string(k++)+","+to_string(edge[{v,n_1}]*100)+","+to_string(y_coor[n_1]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_3)+".\n";

                int source_4=k;
                int source_5=k+1;
                int source_6=k+2;
                int source_7=k+3;  

                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[n_2]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                
                file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_6)+".\n";
                file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_4)+".\n";
                file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_7)+".\n"; 

                if((edge[{v,n_2}]<set[v] && edge[{v,n_3}]<set[v])||(edge[{v,n_2}]>set[v] && edge[{v,n_3}]>set[v]))
                {
                    if(st_element1==4)
                    {
                        int source_8=k;
                        int source_9=k+1;
                        int source_10=k+2;
                        int source_11=k+3;
                        int source_12=k+4;
                        int source_13=k+5;
                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                        file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_9)+".\n";
                        file2<<to_string(j++)+","+to_string(source_11)+","+to_string(source_10)+".\n";
                        file2<<to_string(j++)+","+to_string(source_11)+","+to_string(source_13)+".\n";
                    }
                    else{
                        int source_8=k;
                        int source_9=k+1;
                        int source_10=k+2;
                        int source_11=k+3;
                        int source_12=k+4;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_11)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                        file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_9)+".\n";
                        file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_12)+".\n";
                    }
                }
                else
                {
                    if(st_element1==4)
                    {
                        int source_8=k;
                        int source_9=k+1;
                        int source_10=k+2;
                        int source_11=k+3;
                        int source_12=k+4;

                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[n_3]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_11)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                        file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_9)+".\n";
                        file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_12)+".\n";
                    }
                    else
                    {
                        int source_8=k;
                        int source_9=k+1;
                        int source_10=k+2;
                        int source_11=k+3;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((edge[{v,n_3}])*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                        file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_10)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                        file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_11)+".\n";
                    }
                }               
            }
            
        }
        if(in[v]==4)
        {
            int n_1=in_neighbour[n][3];
            int n_2=in_neighbour[n][2];
            int n_3=in_neighbour[n][1];
            int n_4=in_neighbour[n][0];

            if(n_4==st_order[0])
            {
                st_element1++;
            }

            if(edge[{v,n_1}]==set[v] && edge[{v,n_1}]==set[n_1])
            {
                int source_1=k;
                int source_2=k+1;
                file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";

                int source_3=k;
                int source_4=k+1;
                int source_5=k+2;
                int source_6=k+3;

                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[n_2]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                file2<<to_string(j++)+","+to_string(source_3)+","+to_string(source_5)+".\n";
                file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_3)+".\n";
                file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_6)+".\n";

                if(edge[{v,n_2}]<set[v] && edge[{v,n_3}]>set[v] && edge[{v,n_4}]>set[v] || edge[{v,n_2}]>set[v] && edge[{v,n_3}]>set[v] && edge[{v,n_4}]<set[v] || edge[{v,n_3}]<set[v] && edge[{v,n_4}]>set[v] && edge[{v,n_2}]>set[v] ||edge[{v,n_2}]>set[v] && edge[{v,n_3}]<set[v] && edge[{v,n_4}]<set[v] || edge[{v,n_2}]<set[v] && edge[{v,n_3}]<set[v] && edge[{v,n_4}]>set[v] ||edge[{v,n_3}]>set[v] && edge[{v,n_2}]<set[v] && edge[{v,n_4}]<set[v])
                {
                    int source_7=k;
                    int source_8=k+1;
                    int source_9=k+2;
                    int source_10=k+3;
                    
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                
                    file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_9)+".\n";
                    file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_9)+".\n";
                    file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_10)+".\n";
                    
                    if(st_element1==4)
                    {
                        int source_11=k;
                        int source_12=k+1;
                        int source_13=k+2;
                        int source_14=k+3;
                        int source_15=k+4;
                        int source_16=k+5;

                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_11)+","+to_string(source_15)+".\n";
                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_11)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_16)+".\n";
                    }
                    else
                    {
                        int source_11=k;
                        int source_12=k+1;
                        int source_13=k+2;
                        int source_14=k+3;
                        int source_15=k+4;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string(y_coor[n_4]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_11)+","+to_string(source_14)+".\n";
                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_11)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_15)+".\n";
                    }
                }
                if(edge[{v,n_2}]>set[v] && edge[{v,n_3}]>set[v] && edge[{v,n_4}]>set[v])
                {
                    int source_7=k;
                    int source_8=k+1;
                    int source_9=k+2;
                    int source_10=k+3;
                    int source_11=k+4;

                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                    file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_10)+".\n";
                    file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_7)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_11)+".\n"; 

                    if(st_element1==4)
                    {
                        int source_12=k;
                        int source_13=k+1;
                        int source_14=k+2;
                        int source_15=k+3;
                        int source_16=k+4;
                        int source_17=k+5;
                        int source_18=k+6;

                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_17)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_15)+".\n"; 
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_18)+".\n"; 
                    }
                    else
                    {
                        int source_12=k;
                        int source_13=k+1;
                        int source_14=k+2;
                        int source_15=k+3;
                        int source_16=k+4;
                        int source_17=k+5;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string(y_coor[n_4]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_16)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_17)+".\n";  
                    }      
                }
                if(edge[{v,n_2}]<set[v] && edge[{v,n_3}]<set[v] && edge[{v,n_4}]<set[v])
                {
                   int source_7=k;
                    int source_8=k+1;
                    int source_9=k+2;
                    int source_10=k+3;
                    int source_11=k+4;

                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                    file2<<to_string(j++)+","+to_string(source_7)+","+to_string(source_10)+".\n";
                    file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_7)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_11)+".\n";     

                    if(st_element1==4)
                    {
                        int source_12=k;
                        int source_13=k+1;
                        int source_14=k+2;
                        int source_15=k+3;
                        int source_16=k+4;
                        int source_17=k+5;
                        int source_18=k+6;

                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_17)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_15)+".\n"; 
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_18)+".\n"; 
                    }
                    else
                    {
                        int source_12=k;
                        int source_13=k+1;
                        int source_14=k+2;
                        int source_15=k+3;
                        int source_16=k+4;
                        int source_17=k+5;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string(y_coor[n_4]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_16)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_17)+".\n";  
                    }   
                }
            }
            else{
                int source_1=k;
                int source_2=k+1;
                int source_3=k+2;

                file1<<to_string(k++)+","+to_string(edge[{v,n_1}]*100)+","+to_string(y_coor[n_1]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(set[n_1]*100)+","+to_string(y_coor[n_1]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_2)+".\n";
                file2<<to_string(j++)+","+to_string(source_1)+","+to_string(source_3)+".\n";

                int source_4=k;
                int source_5=k+1;
                int source_6=k+2;
                int source_7=k+3;  

                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[n_2]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(edge[{v,n_2}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                file1<<to_string(k++)+","+to_string(set[n_2]*100)+","+to_string(y_coor[n_2]*100)+",1"+".\n";
                file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                
                file2<<to_string(j++)+","+to_string(source_4)+","+to_string(source_6)+".\n";
                file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_4)+".\n";
                file2<<to_string(j++)+","+to_string(source_5)+","+to_string(source_7)+".\n"; 

                if(edge[{v,n_2}]<set[v] && edge[{v,n_3}]>set[v] && edge[{v,n_4}]>set[v] || edge[{v,n_2}]>set[v] && edge[{v,n_3}]>set[v] && edge[{v,n_4}]<set[v] || edge[{v,n_3}]<set[v] && edge[{v,n_4}]>set[v] && edge[{v,n_2}]>set[v] ||edge[{v,n_2}]>set[v] && edge[{v,n_3}]<set[v] && edge[{v,n_4}]<set[v] || edge[{v,n_2}]<set[v] && edge[{v,n_3}]<set[v] && edge[{v,n_4}]>set[v] ||edge[{v,n_3}]>set[v] && edge[{v,n_2}]<set[v] && edge[{v,n_4}]<set[v])
                {
                    int source_8=k;
                    int source_9=k+1;
                    int source_10=k+2;
                    int source_11=k+3;
                    
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";
                
                    file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_10)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_11)+".\n";
                    
                    if(st_element1==4)
                    {
                        int source_12=k;
                        int source_13=k+1;
                        int source_14=k+2;
                        int source_15=k+3;
                        int source_16=k+4;
                        int source_17=k+5;

                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_16)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_17)+".\n";
                    }
                    else
                    {
                        int source_12=k;
                        int source_13=k+1;
                        int source_14=k+2;
                        int source_15=k+3;
                        int source_16=k+4;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string(y_coor[n_4]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                        file2<<to_string(j++)+","+to_string(source_12)+","+to_string(source_15)+".\n";
                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_12)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_16)+".\n";
                    }
                }
                if(edge[{v,n_2}]>set[v] && edge[{v,n_3}]>set[v] && edge[{v,n_4}]>set[v])
                {
                    int source_8=k;
                    int source_9=k+1;
                    int source_10=k+2;
                    int source_11=k+3;
                    int source_12=k+4;

                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                    file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_11)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                    file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_9)+".\n";
                    file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_12)+".\n"; 

                    if(st_element1==4)
                    {
                        int source_13=k;
                        int source_14=k+1;
                        int source_15=k+2;
                        int source_16=k+3;
                        int source_17=k+4;
                        int source_18=k+5;
                        int source_19=k+6;

                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_18)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_15)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_17)+","+to_string(source_16)+".\n"; 
                        file2<<to_string(j++)+","+to_string(source_17)+","+to_string(source_19)+".\n"; 
                    }
                    else
                    {
                        int source_13=k;
                        int source_14=k+1;
                        int source_15=k+2;
                        int source_16=k+3;
                        int source_17=k+4;
                        int source_18=k+5;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string(y_coor[n_4]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]-1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_17)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_15)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_18)+".\n";  
                    }      
                }
                if(edge[{v,n_2}]<set[v] && edge[{v,n_3}]<set[v] && edge[{v,n_4}]<set[v])
                {
                    int source_8=k;
                    int source_9=k+1;
                    int source_10=k+2;
                    int source_11=k+3;
                    int source_12=k+4;

                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string(y_coor[n_3]*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(edge[{v,n_3}]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string((y_coor[v]+1)*100)+",0"+".\n";
                    file1<<to_string(k++)+","+to_string(set[n_3]*100)+","+to_string(y_coor[n_3]*100)+",1"+".\n";
                    file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n";

                    file2<<to_string(j++)+","+to_string(source_8)+","+to_string(source_11)+".\n";
                    file2<<to_string(j++)+","+to_string(source_9)+","+to_string(source_8)+".\n";
                    file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_9)+".\n";
                    file2<<to_string(j++)+","+to_string(source_10)+","+to_string(source_12)+".\n";     

                    if(st_element1==4)
                    {
                        int source_13=k;
                        int source_14=k+1;
                        int source_15=k+2;
                        int source_16=k+3;
                        int source_17=k+4;
                        int source_18=k+5;
                        int source_19=k+6;

                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[n_4]-1)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_18)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_15)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_17)+","+to_string(source_16)+".\n"; 
                        file2<<to_string(j++)+","+to_string(source_17)+","+to_string(source_19)+".\n"; 
                    }
                    else
                    {
                        int source_13=k;
                        int source_14=k+1;
                        int source_15=k+2;
                        int source_16=k+3;
                        int source_17=k+4;
                        int source_18=k+5;

                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string(y_coor[n_4]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(edge[{v,n_4}]*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string((y_coor[v]+2)*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string((set[v]+1)*100)+","+to_string(y_coor[v]*100)+",0"+".\n";
                        file1<<to_string(k++)+","+to_string(set[n_4]*100)+","+to_string(y_coor[n_4]*100)+",1"+".\n";
                        file1<<to_string(k++)+","+to_string(set[v]*100)+","+to_string(y_coor[v]*100)+",1"+".\n"; 

                        file2<<to_string(j++)+","+to_string(source_13)+","+to_string(source_17)+".\n";
                        file2<<to_string(j++)+","+to_string(source_14)+","+to_string(source_13)+".\n";
                        file2<<to_string(j++)+","+to_string(source_15)+","+to_string(source_14)+".\n";
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_15)+".\n";  
                        file2<<to_string(j++)+","+to_string(source_16)+","+to_string(source_18)+".\n";  
                    }   
                }       
            }
            
        }
    }
    file1.close();
    file2.close();
}

int main() {
   // Number of vertices in the graph
    int V;
    cout<<"enter number of vertices"<<endl;
    cin>>V;

    // Adjacency list representation of the graph
    vector<vector<int>>adj(V);
    
    cout<<"give the name of the file where node and edge details are there"<<endl;
    string details;
    cin>>details;

    read_graph(details,adj);//to get graph details from txt file

    int s,t;
    cout<<"give input for s and t which should be adjacent"<<endl;

    cin>>s;
    cin>>t;

    // if(!isadjacent(adj,s,t))
    // {
    //     adj[s].push_back(t);
    //     adj[t].push_back(s);
    // }
   
    embeding(V,adj,s,t);

    return 0;
}