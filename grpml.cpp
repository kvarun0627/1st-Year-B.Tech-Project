#include<bits/stdc++.h>
#include<fstream>
#include<sstream>

using namespace std;

int main()
{
    string h;
    cout<<"give file name"<<endl;
    cin>>h;

    ofstream file;
    file.open(h);

    file<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
          "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" xmlns:java=\"http://www.yworks.com/xml/yfiles-common/1.0/java\" xmlns:sys=\"http://www.yworks.com/xml/yfiles-common/markup/primitives/2.0\" xmlns:x=\"http://www.yworks.com/xml/yfiles-common/markup/2.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:y=\"http://www.yworks.com/xml/graphml\" xmlns:yed=\"http://www.yworks.com/xml/yed/3\" xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://www.yworks.com/xml/schema/graphml/1.1/ygraphml.xsd\">\n"
          "  <!--Created by yEd 3.21.1-->\n"
          "  <key attr.name=\"Description\" attr.type=\"string\" for=\"graph\" id=\"d0\"/>\n"
          "  <key for=\"port\" id=\"d1\" yfiles.type=\"portgraphics\"/>\n"
          "  <key for=\"port\" id=\"d2\" yfiles.type=\"portgeometry\"/>\n"
          "  <key for=\"port\" id=\"d3\" yfiles.type=\"portuserdata\"/>\n"
          "  <key attr.name=\"url\" attr.type=\"string\" for=\"node\" id=\"d4\"/>\n"
          "  <key attr.name=\"description\" attr.type=\"string\" for=\"node\" id=\"d5\"/>\n"
          "  <key for=\"node\" id=\"d6\" yfiles.type=\"nodegraphics\"/>\n"
          "  <key for=\"graphml\" id=\"d7\" yfiles.type=\"resources\"/>\n"
          "  <key attr.name=\"url\" attr.type=\"string\" for=\"edge\" id=\"d8\"/>\n"
          "  <key attr.name=\"description\" attr.type=\"string\" for=\"edge\" id=\"d9\"/>\n"
          "  <key for=\"edge\" id=\"d10\" yfiles.type=\"edgegraphics\"/>\n"
          "  <graph edgedefault=\"directed\" id=\"G\">\n"
          "    <data key=\"d0\"/>\n";

        string a;
        cout<<"give  name of the file where details of the nodes are present"<<endl;
        cin>>a;

        ifstream file1;
        file1.open(a);
        string line;
        while(getline(file1,line))
        {
            string vertices="";
            string x_coor="";
            string y_coor="";
            int flag;
            int i=0;
            while(line[i]!=',')
            {
                vertices+=line[i];
                i++;
            }
            i++;

            while(line[i]!=',')
            {
                x_coor+=line[i];
                i++;
            }
            i++;

            while(line[i]!=',')
            {
                y_coor+=line[i];
                i++;
            }
            i++;
            flag=line[i]-'0';
            cout<<flag;
            file<<"      <node id=\"n"+vertices+"\">\n"
                  "        <data key=\"d6\">\n"
                  "          <y:ShapeNode>\n"
                  "            <y:Geometry height=\"30.0\" width=\"30.0\" "+"x=\""+x_coor+"\""+" "+"y=\""+y_coor+"\"/>\n";
            if(flag==1)
            {
                file<<"            <y:Fill color=\"#FFCC00\" transparent=\"false\"/>\n";
            }
            else
            {
                file<<"            <y:Fill color=\"#FFFFFF\" transparent=\"false\"/>\n";
            }
            file<<"            <y:BorderStyle color=\"#000000\" raised=\"false\" type=\"line\" width=\"1.0\"/>\n"
                  "            <y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"20\" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" hasText=\"false\" height=\"4.0\" horizontalTextPosition=\"center\" iconTextGap=\"4\" modelName=\"custom\" textColor=\"#000000\" verticalTextPosition=\"bottom\" visible=\"true\" width=\"4.0\" x=\"13.0\" y=\"13.0\">\n"
                  "              <y:LabelModel>\n"
                  "                <y:SmartNodeLabelModel distance=\"4.0\"/>\n"
                  "              </y:LabelModel>\n"
                  "              <y:ModelParameter>\n"
                  "                <y:SmartNodeLabelModelParameter labelRatioX=\"0.0\" labelRatioY=\"0.0\" nodeRatioX=\"0.0\" nodeRatioY=\"0.0\" offsetX=\"0.0\" offsetY=\"0.0\" upX=\"0.0\" upY=\"-1.0\"/>\n"
                  "              </y:ModelParameter>\n"
                  "            </y:NodeLabel>\n"
                  "            <y:Shape type=\"ellipse\"/>\n"
                  "          </y:ShapeNode>\n"
                  "        </data>\n"
                  "      </node>\n"; 
                  
            // if(flag)
            // {
            //   file<<"            <y:Fill color=\"#FFCC00\" transparent=\"false\"/>\n"
            //         "            <y:BorderStyle color=\"#000000\" raised=\"false\" type=\"line\" width=\"1.0\"/>\n"
            //         "            <y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"20\" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" hasText=\"false\" height=\"4.0\" horizontalTextPosition=\"center\" iconTextGap=\"4\" modelName=\"custom\" textColor=\"#000000\" verticalTextPosition=\"bottom\" visible=\"true\" width=\"4.0\" x=\"13.0\" y=\"13.0\">\n"
            //         "              <y:LabelModel>\n"
            //         "                <y:SmartNodeLabelModel distance=\"4.0\"/>\n"
            //         "              </y:LabelModel>\n"
            //         "              <y:ModelParameter>\n"
            //         "                <y:SmartNodeLabelModelParameter labelRatioX=\"0.0\" labelRatioY=\"0.0\" nodeRatioX=\"0.0\" nodeRatioY=\"0.0\" offsetX=\"0.0\" offsetY=\"0.0\" upX=\"0.0\" upY=\"-1.0\"/>\n"
            //         "              </y:ModelParameter>\n"
            //         "            </y:NodeLabel>\n"
            //         "            <y:Shape type=\"ellipse\"/>\n"
            //         "          </y:ShapeNode>\n"
            //         "        </data>\n"
            //         "      </node>\n";
            // }
            // else
            // {
            
            //   file<<"            <y:Fill color=\"#FFFFFF\" transparent=\"false\"/>\n"
            //         "            <y:BorderStyle color=\"#000000\" raised=\"false\" type=\"line\" width=\"1.0\"/>\n"
            //         "            <y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"20\" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" hasText=\"false\" height=\"4.0\" horizontalTextPosition=\"center\" iconTextGap=\"4\" modelName=\"custom\" textColor=\"#000000\" verticalTextPosition=\"bottom\" visible=\"true\" width=\"4.0\" x=\"13.0\" y=\"13.0\">\n"
            //         "              <y:LabelModel>\n"
            //         "                <y:SmartNodeLabelModel distance=\"4.0\"/>\n"
            //         "              </y:LabelModel>\n"
            //         "              <y:ModelParameter>\n"
            //         "                <y:SmartNodeLabelModelParameter labelRatioX=\"0.0\" labelRatioY=\"0.0\" nodeRatioX=\"0.0\" nodeRatioY=\"0.0\" offsetX=\"0.0\" offsetY=\"0.0\" upX=\"0.0\" upY=\"-1.0\"/>\n"
            //         "              </y:ModelParameter>\n"
            //         "            </y:NodeLabel>\n"
            //         "            <y:Shape type=\"ellipse\"/>\n"
            //         "          </y:ShapeNode>\n"
            //         "        </data>\n"
            //         "      </node>\n";
            // }
        }
        file1.close();

        string j;
        cout<<"give name of the file where details of the edges are present"<<endl;
        cin>>j;

        ifstream file2;
        file2.open(j);

        while(getline(file2,line))
        {
            string id="";
            string source="";
            string target="";

            int i=0;
            while(line[i]!=',')
            {
                id+=line[i];
                i++;
            }
            i++;

            while(line[i]!=',')
            {
                source+=line[i];
                i++;
            }
            i++;

            while(line[i]!='.')
            {
                target+=line[i];
                i++;
            }

             file<<"    <edge id=\"e"+id+"\""+" "+"source=\"n"+source+"\""+" "+"target=\"n"+target+"\""+">\n"
                   "      <data key=\"d10\">\n"
                   "        <y:PolyLineEdge>\n"
                   "          <y:Path sx=\"0.0\" sy=\"0.0\" tx=\"0.0\" ty=\"0.0\"/>\n"
                   "          <y:LineStyle color=\"#0000FF\" type=\"line\" width=\"2.0\"/>\n"
                   "          <y:Arrows source=\"none\" target=\"none\"/>\n"
                   "          <y:BendStyle smoothed=\"false\"/>\n"
                   "        </y:PolyLineEdge>\n"
                   "      </data>\n"
                   "    </edge>\n";
        }
        file2.close();

        file<<"  </graph>\n"
              "  <data key=\"d7\">\n"
              "    <y:Resources/>\n"
              "  </data>\n"
              "</graphml>";
}