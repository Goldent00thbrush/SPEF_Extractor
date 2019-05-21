#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;
string design_name,bus_del,divd;
class map{      //map to keep track of ports
    struct single{
        int num;
        string name;
    };
    single *m;
    int count=0,pn=0;
public:
    map(){
        m=new single [10000];
        for(int i=0;i<10000;i++)
        {
            m[i].name="";
            m[i].num=-1;
        }
    }
    bool add(string s){
       if(search(s)!=-1) return false;
        pn++;
        m[count].num=pn;

        m[count].name=s;
        count++;
        return true;
    }
    int search(string s){
        for (int i=0;i<2000;i++)
            if (m[i].name==s) return m[i].num;
        return -1;
    }
};
int nets_size = 5000;
struct  PINS
{
    string name;
    string pin_state;
    string direction; //I/O
    int firstcoordinate;
    int secondcoordinate;
};
vector <PINS> pins;
struct VIA {
    string name; //the VIA Name
    string resistance; //the VIA resistance
};
vector < VIA > via; //ALL the VIAS

vector <float> segmentscapacitances;
vector <float> segmentsresistances;
struct Layer {
    string name; //Layer Name
    string resistance_unit; //Layer resistance unit (RPERSQ Mostly)
    string resistance_value; //Layer resistance value
    string capacitance_unit; //Layer Capacitance unit (CPERSQDIST Mostly)
    string capacitance_value; //Layer Capacitance value
    string edge_capacitance; //Layer edge capacitance
    string width;
    string spacing;

};
vector < Layer > layers; //those are the layers of TYPE ROUTING only
vector < string > all_layers; //those are all the layers in a file
struct components
{
    string name;
    int fcor,scor;
};
vector <components> comps;

struct coor
{
    string x, y;
};

struct conn {
    string layer;
    vector<coor> cord;

};
struct NETS
{
    string name;
    vector <string> p_type;
    vector <string> p_name;
    vector<conn> connection;
    vector <string> vias;
};
NETS nets[5000];
void searchCoordinates(int i, int first_index, int second_index, int &f, int &s){
    f=-1;s=-1;
    for (int j=0;j<nets[i].connection.size();j++){
            for (int k=0;k<nets[i].connection[j].cord.size();k++){
                if (nets[i].connection[j].cord[k].y=="*" && k-1>0)
                    nets[i].connection[j].cord[k].y=nets[i].connection[j].cord[k-1].y;
                if (nets[i].connection[j].cord[k].x=="*" && k-1>0)
                    nets[i].connection[j].cord[k].x=nets[i].connection[j].cord[k-1].x;
                          if ((first_index == stoi(nets[i].connection[j].cord[k].x))
                        && (second_index == stoi( nets[i].connection[j].cord[k].y))){
                    f=j;s=k;
                    return;
                        //return nets[i].connection[j].cord;
        }
    }
}
return;}
void find_cr(string name, int &fcor, int &scor)
{
    for (int r=0;r<5000;r++)
    {
        if(name==comps[r].name)
        {
            fcor=comps[r].fcor;
            scor=comps[r].scor;
            return;
        }
    }
    fcor=-1;
    scor=-1;
}
void readfromDEF()
{
    bool designfound = false,pinsfound = false, netsfound = false, finish2 = false;
    ifstream DEF;
    string word;
    int nets_size = 0;
    DEF.open("D://College//Semester 10-- Spring 2019//CSCE3304 - Digital Design II//Assignments//pro2//untitled//cpu2.def");

    if (DEF.is_open())
    {


        while (!DEF.eof())
        {
            DEF >> word;
            if ((word == "DESIGN") && (designfound == false)) {
                            DEF >> word;
                            design_name = word;
                            designfound = true;
                        }
            if ((word == "BUSBITCHARS") ) {
                            DEF >> word;
                            bus_del = word.substr(1,word.size()-2);

                        }
            if ((word == "DIVIDERCHAR") ) {
                            DEF >> word;
                            divd = word.substr(1,word.size()-2);

                        }
            if (word=="COMPONENTS")
            {
                int n=0;
              DEF>>word;
              DEF>>word;
              DEF >> word;
              components cq;
              while (word != "END")
              {
              if (word == "-")
              {
                  DEF>>word;
                  cq.name=word;
                  DEF>>word;
                  DEF>>word;
                  DEF>>word;
                  DEF>>word;
                  DEF>>word;

cq.fcor=stoi(word);
DEF>>word;
cq.scor=stoi(word);
comps.push_back(cq);
               n++;

               DEF>>word;
               DEF>>word;
               DEF>>word;
               DEF>>word;
if (word==";") DEF>>word;
cq.name.clear();
cq.fcor=0;
cq.scor=0;
              }}
DEF>>word;
for (int p=0;p<comps.size();p++)
    cout<<comps[p].name<<"  "<<comps[p].fcor<<" "<<comps[p].scor<<endl;
            }
            if (word == "PINS")
            {
                int index = 0;

                while (pinsfound == false)
                {
                    DEF >> word;
                    if (word == "-")
                    {

                        pins.push_back({ " "," "," ",0,0 });
                        DEF >> word;
                        pins[index].name = word;
                    }
                    if (word == "DIRECTION") {
                        DEF >> word;
                        pins[index].direction = word;
                    }
                    if ((word == "PLACED") || (word == "FIXED"))
                    {
                        pins[index].pin_state = word;
                        DEF >> word;
                        DEF >> word;
                        pins[index].firstcoordinate = stoi(word);

                        DEF >> word;
                        pins[index].secondcoordinate = stoi(word);
                        index++;
                    }
                    if (word == "END")
                        pinsfound = true;

                }
            }
            if (word == "NETS")
            {
                int index2 = 0;
                int metals_size = 0, firstc = 0;
                coor r;
                conn c;
                int j = 0, k = 0;
                DEF >> word;
                nets_size=stoi(word);
                DEF >> word;
                while (word != "END")
                {
                    DEF >> word;

                    if (word == "-")
                    {
                        DEF >> word;
                        nets[index2].name = word;
                        j = 0;
                        nets_size++;
                        while (word != "+")
                        {
                            DEF >> word;
                            if (word == "(")
                            {
                                DEF >> word;
                                nets[index2].p_type.push_back({ word });
                                DEF >> word;
                                nets[index2].p_name.push_back({ word });
                            }

                        }
                    }
                    if ((word == "ROUTED") || (word == "NEW"))
                    {
                        DEF >> word;
                        c.layer = word;
                        //nets[index2].connection[i].layer=word;
                    }
                    if (word == "(")
                    {
bracket:                        DEF >> word;
                        r.x = word;
                        // nets[index2].connection[i].cord[j].x=word;
                        firstc++;
                        DEF >> word;
                        r.y = word;
                        // nets[index2].connection[i].cord[j].y=word;
                        j++;
                        c.cord.push_back(r);

                    }
                    if (word == ")")
                    {
                        DEF >> word;

                        if (word=="(") goto bracket;

                        if ((word != "NEW") && (word != "(") && (word != ";") && (word != "+"))
                        {

                            nets[index2].vias.push_back(word);
                        nets[index2].connection.push_back(c);
                        conn p;
                        c=p;
                            k++;

                        }
                      else  if ((word == "NEW") || (word == ";"))
                       {  nets[index2].connection.push_back(c);

                            conn p;
                            c=p;
                }

                    }
                    if (word == ";")
                    {

                        index2++;
                    }
                   // cout<<nets[index2].name<<" "<<nets[index2].connection.size()<<endl;
                }
              //  nets[index2].connection.push_back(c);



            if(word=="END")break;

            }

        }
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < nets[i].p_name.size(); j++)
            {
                cout << nets[i].p_type[j];
            }
            cout << endl;
        }

    }
    else
    {
        cout << "cant open file" << endl;
    }


}

void readfromLEF() {
    string x, x1, x2, x3, x4, width_l, name, space;
    bool done = false;
    int count = 0;
    ifstream LEF;
    LEF.open("D://College//Semester 10-- Spring 2019//CSCE3304 - Digital Design II//Assignments//pro2//untitled//tmp.lef");
    if (LEF.is_open()) {
        while (!LEF.eof()) {
            LEF >> x;
            if (x == "LAYER") {

                LEF >> name;
                all_layers.push_back(name);
                sort(all_layers.begin(), all_layers.end());
                all_layers.erase(unique(all_layers.begin(), all_layers.end()), all_layers.end());
                LEF >> x;
                LEF >> x;

                if (x == "ROUTING") {
                    done = false;
                    do {
                        LEF >> x;
                        if (x == "WIDTH")
                        {
                            LEF >> width_l;

                        }
                        if (x == "SPACING")
                        {
                            LEF >> space;
                        }
                        if (x == "RESISTANCE") {
                            LEF >> x1; //units
                            LEF >> x2; //value
                        }
                        if (x == "CAPACITANCE") {
                            LEF >> x3;
                            LEF >> x4;
                            count++;
                        }
                        if (x == "EDGECAPACITANCE") {
                            LEF >> x;
                            layers.push_back({ name,x1,x2,x3,x4,x,width_l,space });
                            done = true;
                        }

                    } while (done == false);
                }
            }
            if (x == "VIA") {

                string res, name2;
                bool flag = false;
                LEF >> name2;
                do {
                    LEF >> x;
                    if (x == "RESISTANCE") {
                        LEF >> res;
                        via.push_back({ name2,res });
                        flag = true;
                    }
                } while (flag == false);
            }
        }
    }
    else {
        cout << "unable to open LEF file" << endl;
    }
    LEF.close();
}


float calculateSegmentLength(int first_coordinate_x, int second_coordinate_x, int first_coordinate_y, int second_coordinate_y) {
   cout<<"x1: "<<first_coordinate_x<<"x2: "<<second_coordinate_x<<"y "<<sqrt(pow(first_coordinate_x + second_coordinate_x, 2))<<endl;
    return	sqrt(pow(first_coordinate_x + second_coordinate_x, 2) + pow(first_coordinate_y + second_coordinate_y, 2));
}
float calculateSegmentCapacitance(float width, float spacing, float capacitance, float edge_capacitance) { //area*CPERSQDIST+length*EDGECAP from LEF
    return (width*spacing*capacitance) + (spacing*edge_capacitance); // (WIDTH*SPACING*CPERSQDIST)+(SPACING*EDGECAP)
}
float calculateSegmentResistance(float sheet_resistance, float length, float width) { //per square //taken from LEF
    return sheet_resistance * (length / width); //parasitic_resistance
}
float calculateViaResistance(float via_resistance) {
    return via_resistance;
}
float capacitanceBetweenNodes(int first_node_index, int second_node_index, vector<float>segmentscapacitances) {
    float capacitance_between_nodes = 0.0;
    for (int i = first_node_index; i<second_node_index;i++){
        capacitance_between_nodes+=segmentscapacitances[i];
    }
    return capacitance_between_nodes;
}

void find_idx(int i,coor c, int& f, int& s)//net index
{
    for(int j=0;j<nets[i].connection.size();j++)
    {
        for (int k=0;k<nets[i].connection[j].cord.size();k++)
        {
            if (c.x==nets[i].connection[j].cord[k].x  && c.y==nets[i].connection[j].cord[k].y)
            {
                f=j;s=k;
                return;
            }
        }
    }
}
string find_layerW(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].width;
    }
}
string find_layerR(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].resistance_value;
    }
}
string find_layerC(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].capacitance_value;
    }
}
string find_layerE(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].edge_capacitance;
    }
}
float resistanceBetweenNodes(int j, int k, int l, int k2, int p) {
    float r2=0;
    int t2=r2;
    while (true)
    {
        if(k==k2)
        {
            string w= find_layerW(nets[j].connection[k].layer);
            string r=find_layerR(nets[j].connection[k].layer);
            if (k2==t2){
                for (int i=l;i<=p;i++){
                    if (i+1>p) break;
                    if (nets[j].connection[k].cord[i+1].y=="*")
                        nets[j].connection[k].cord[i+1].y=nets[j].connection[k].cord[i].y;
                    if (nets[j].connection[k].cord[i+1].x=="*")
                        nets[j].connection[k].cord[i+1].x=nets[j].connection[k].cord[i].x;
                    if (nets[j].connection[k].cord[i].y=="*")
                        nets[j].connection[k].cord[i].y=nets[j].connection[k].cord[i+1].y;
                    if (nets[j].connection[k].cord[i].x=="*")
                        nets[j].connection[k].cord[i].x=nets[j].connection[k].cord[i+1].x;
                r2+=calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)) ;
            }
            }
            else {
            for (int i=l;i<nets[j].connection[k].cord.size();i++){
                if (i+1>=nets[j].connection[k].cord.size()) break;
                if (nets[j].connection[k].cord[i+1].y=="*")
                    nets[j].connection[k].cord[i+1].y=nets[j].connection[k].cord[i].y;
                if (nets[j].connection[k].cord[i+1].x=="*")
                    nets[j].connection[k].cord[i+1].x=nets[j].connection[k].cord[i].x;
                if (nets[j].connection[k].cord[i].y=="*")
                    nets[j].connection[k].cord[i].y=nets[j].connection[k].cord[i+1].y;
                if (nets[j].connection[k].cord[i].x=="*")
                    nets[j].connection[k].cord[i].x=nets[j].connection[k].cord[i+1].x;
            r2+=calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)) ;
        }
         }
             break;
        }
        else if (k<k2)
        {
            string w= find_layerW(nets[j].connection[k2].layer);
            string r=find_layerR(nets[j].connection[k2].layer);
            if (k2==t2){
                for (int i=0;i<=p;i++){
                    if (i+1>p) break;
                    if (nets[j].connection[k2].cord[i+1].y=="*")
                        nets[j].connection[k2].cord[i+1].y=nets[j].connection[k].cord[i].y;
                    if (nets[j].connection[k2].cord[i+1].x=="*")
                        nets[j].connection[k2].cord[i+1].x=nets[j].connection[k].cord[i].x;
                    if (nets[j].connection[k2].cord[i].y=="*")
                        nets[j].connection[k2].cord[i].y=nets[j].connection[k].cord[i+1].y;
                    if (nets[j].connection[k2].cord[i].x=="*")
                        nets[j].connection[k2].cord[i].x=nets[j].connection[k].cord[i+1].x;
                r2+=calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)) ;
            }}
                else {
                for (int i=0;i<nets[j].connection[k].cord.size();i++){
                    if (i+1>=nets[j].connection[k].cord.size()) break;
                    if (nets[j].connection[k].cord[i+1].y=="*")
                        nets[j].connection[k].cord[i+1].y=nets[j].connection[k].cord[i].y;
                    if (nets[j].connection[k].cord[i+1].x=="*")
                        nets[j].connection[k].cord[i+1].x=nets[j].connection[k].cord[i].x;
                    if (nets[j].connection[k].cord[i].y=="*")
                        nets[j].connection[k].cord[i].y=nets[j].connection[k].cord[i+1].y;
                    if (nets[j].connection[k].cord[i].x=="*")
                        nets[j].connection[k].cord[i].x=nets[j].connection[k].cord[i+1].x;
                r2+=calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)) ;
            }
            }

            k2--;
        }
        //vias

        return r2;
    }

}
float lumpedCapacitance(int i)
{
    float l=0;
    for (int k = 0; k < nets[i].connection.size(); k++) {// per segment
                     string w= find_layerW(nets[i].connection[k].layer);
                     string c=find_layerC(nets[i].connection[k].layer);
                     string e =find_layerE(nets[i].connection[k].layer);
                     for (int j=0;j<nets[i].connection[k].cord.size();j++){
                         if (j+1>=nets[i].connection[k].cord.size()) break;
                         if (nets[i].connection[k].cord[j+1].y=="*")
                             nets[i].connection[k].cord[j+1].y=nets[i].connection[k].cord[j].y;
                         if (nets[i].connection[k].cord[j+1].x=="*")
                             nets[i].connection[k].cord[j+1].x=nets[i].connection[k].cord[j].x;
                         if (nets[i].connection[k].cord[j].y=="*")
                             nets[i].connection[k].cord[j].y=nets[i].connection[k].cord[j+1].y;
                         if (nets[i].connection[k].cord[j].x=="*")
                             nets[i].connection[k].cord[j].x=nets[i].connection[k].cord[j+1].x;
                      l+=calculateSegmentCapacitance(stof(w), calculateSegmentLength(stoi(nets[i].connection[k].cord[j].x), stoi(nets[i].connection[k].cord[j+1].x), stoi(nets[i].connection[k].cord[j].y), stoi(nets[i].connection[k].cord[j+1].y)),  stof(c), stof(e));

                     }
}
    return l;
}
float Resistance2 (int j){
    float r2=0;
    for (int k = 0; k < nets[j].connection.size()-1; k++) {// per segment
       string w= find_layerW(nets[j].connection[k].layer);
       string r=find_layerR(nets[j].connection[k].layer);
       for (int i=0;i<nets[j].connection[k].cord.size();i++){
           if (i+1>=nets[j].connection[k].cord.size()) break;
           if (nets[j].connection[k].cord[i+1].y=="*")
               nets[j].connection[k].cord[i+1].y=nets[j].connection[k].cord[i].y;
           if (nets[j].connection[k].cord[i+1].x=="*")
               nets[j].connection[k].cord[i+1].x=nets[j].connection[k].cord[i].x;
           if (nets[j].connection[k].cord[i].y=="*")
               nets[j].connection[k].cord[i].y=nets[j].connection[k].cord[i+1].y;
           if (nets[j].connection[k].cord[i].x=="*")
               nets[j].connection[k].cord[i].x=nets[j].connection[k].cord[i+1].x;
       r2+=calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)) ;

       }
    }
    for (int i=0;i<nets[j].vias.size();i++)
    {
        for (int k=0;k<via.size();k++)
        {
            if (nets[j].vias[i]==via[k].name)
            {r2+=stof(via[k].resistance); break;}
        }
    }
    return r2;
}
void write(string filename)
{
    ofstream ofile;
    ofile.open (filename,ios::binary | ios::in);

        if (ofile.is_open()){
            ofile.clear();

            string str;
            int len;
            /*  Header Section begin  */
            str="*SPEF \"IEEE 1481-2009\"\n"; //Spef version
             len=str.length();
            ofile.write(str.c_str(),len);
            str="*DESIGN \""+design_name+"\"\n"; //design name
             len=str.length();
            ofile.write(str.c_str(),len);
            str="*DATE \""; //date
            len=str.length();
            ofile.write(str.c_str(),len);
            auto t = std::time(nullptr);
             auto tm = *std::localtime(&t);
           std::ostringstream oss;
            oss << put_time(&tm, "%d-%m-%Y");
             auto sr = oss.str();
             len=sr.length();
             ofile.write(sr.c_str(),len);
             ofile.write("\"\n",2);
             str="*VENDOR \"SPEF Generator\"\n"; //
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*PROGRAM \"SPEF Generator\"\n"; //RC EXtrcation tool
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*VERSION \"1.1.0\"\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*DESIGN_FLOW \"NETLIST_TYPE_VERILOG\"\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*DIVIDER "+divd+"\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*DELIMITER :\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*BUS_DELIMITER "+bus_del+"\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*T_UNIT 1 NS\n"; //time units
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*C_UNIT 1 PF\n";  //capcaitence units
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*R_UNIT 1 OHM\n"; //resistence units
              len=str.length();
             ofile.write(str.c_str(),len);
             str="*L_UNIT 1 HENRY\n"; //inductence units
              len=str.length();
             ofile.write(str.c_str(),len);
             /* Header Section end  */

             /* Nmap Section begin */
             map m;
             str="\n*NAME_MAP\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             for (int i=0;i<pins.size();i++)
             {
                if ( m.add(pins[i].name)){
                 str="*"+to_string(m.search(pins[i].name))+" "+pins[i].name+"\n";
                 len=str.length();
                 ofile.write(str.c_str(),len);
                }
             }
             for (int i=0;i<nets_size;i++)
             {
                 if (m.add(nets[i].name)){
                 str="*"+to_string(m.search(nets[i].name))+" "+nets[i].name+"\n";
                 len=str.length();
                 ofile.write(str.c_str(),len);
             }}
             /* Nmap Section end */

             /* Ports Section begin */   //primary inputs and outputs
             str="\n*PORTS\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             for (int i=0;i<pins.size();i++)
             {
                if (pins[i].direction=="INPUT"){
                 str="*"+to_string(m.search(pins[i].name))+" I\n";
                 len=str.length();
                 ofile.write(str.c_str(),len);
                }
                else if (pins[i].direction=="OUTPUT"){
                 str="*"+to_string(m.search(pins[i].name))+" O\n";
                 len=str.length();
                 ofile.write(str.c_str(),len);
                }
                else if (pins[i].direction==" "){
                    str="*"+to_string(m.search(pins[i].name))+"\n";
                    len=str.length();
                    ofile.write(str.c_str(),len);
                }
             }
             /* Ports Section end */

             /* detailed net Section begin */ //repeat based on number of nets
            for(int i=0;i<nets_size;i++){
               int x=m.search(nets[i].name);
               if (x!=-1){
             str="\n*D_NET *"+to_string(x);
             len=str.length();
            ofile.write(str.c_str(),len);
             float c=lumpedCapacitance(i);
             str=" "+to_string(c)+"\n";
             len=str.length();
            ofile.write(str.c_str(),len);
             str="*CONN\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             for (int j=0;j<nets[i].p_name.size();j++)
             {
                 string type="";
                 if(nets[i].p_type[j]=="PIN"){
                     for(int i=0;i<pins.size();i++)
                         if (pins[i].name==nets[i].p_name[j]){type=pins[i].direction;break;}
                     if (type=="INPUT") type="I";
                     if (type=="OUTPUT") type="O";
                     if ( m.add(nets[i].p_name[j])){
                 str="*P *"+to_string(m.search(nets[i].p_name[j]))+" "+type+"\n";
                  len=str.length();
                 ofile.write(str.c_str(),len);
                     }
                     else {
                         str="*P "+nets[i].p_name[j]+" "+type+"\n";
                          len=str.length();
                         ofile.write(str.c_str(),len);
                     }}
                 else {

                     if ( m.add(nets[i].p_type[j])){
                     str="*I *"+to_string(m.search(nets[i].p_type[j]))+":"+nets[i].p_name[j]+" "+type+"\n";
                          len=str.length();
                         ofile.write(str.c_str(),len);
                    }
                     else {
                         str="*I "+nets[i].p_type[j]+":"+nets[i].p_name[j]+" "+type+"\n";
                              len=str.length();
                             ofile.write(str.c_str(),len);
                     }
             }
             }
             str="*CAP\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             if (nets[i].p_name.size()==2)
             {float q=lumpedCapacitance(i);
                 if (nets[i].p_type[0]=="PIN")
                 str="1 "+nets[i].p_name[0]+" ";
                 else
                     str="1 "+nets[i].p_type[0]+":"+nets[i].p_name[0]+" ";
                 if (nets[i].p_type[1]=="PIN")
                         str+=nets[i].p_name[1]+" "+to_string(q)+"\n";
                 else
                     str+=nets[i].p_type[1]+":"+nets[i].p_name[1]+" "+to_string(q)+"\n";
                 len=str.length();
                ofile.write(str.c_str(),len);
             }


             str="*RES\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             if (nets[i].p_name.size()==2)
             {float q=Resistance2(i);
                 if (nets[i].p_type[0]=="PIN")
                 str="1 "+nets[i].p_name[0]+" ";
                 else
                     str="1 "+nets[i].p_type[0]+":"+nets[i].p_name[0]+" ";
                 if (nets[i].p_type[1]=="PIN")
                         str+=nets[i].p_name[1]+" "+to_string(q)+"\n";
                 else
                     str+=nets[i].p_type[1]+":"+nets[i].p_name[1]+" "+to_string(q)+"\n";
                 len=str.length();
                ofile.write(str.c_str(),len);
             }
             else {
                 int count=1;
                for(int j=0;j<nets[i].p_name.size();j++)//combinations
                {
                    for(int k=j+1;k<nets[i].p_name.size();k++)//combinations
                    {
                       if (k>=nets[i].p_name.size()) break;
                        int f=0,s=0,l1=0,m1=0,l2=0,m2=0,r=0;
                        if (nets[i].p_type[j]!="PIN")
                                 find_cr(nets[i].p_type[j],f,s);
                        if (f==-1) continue;
                        searchCoordinates(i,f,s,l1,m1);
                        if (l1==-1 )continue;
                        if (nets[i].p_type[k]!="PIN")
                                 find_cr(nets[i].p_type[k],f,s);
                        if (f==-1) continue;
                        searchCoordinates(i,f,s,l2,m2);
                        if (l2==-1 || f==-1)continue;
                       // find_idx(i,c1[0],l1,m1);
                       // find_idx(i,c2[0],l2,m2);
                       r= resistanceBetweenNodes(i,l1,m1,l2,m2);
                       str=to_string(count)+" "+nets[i].p_type[j]+":"+nets[i].p_name[j]+" "+nets[i].p_type[k]+":"+nets[i].p_name[k]+" "+to_string(r) +" \n";
                       len=str.length();
                      ofile.write(str.c_str(),len);
                       count++;
                    }
}
                }
             str="*END\n";
              len=str.length();
             ofile.write(str.c_str(),len);
             /* detailed net Section end */
               }
        }
       }
        else {
            cout<<"unable to open output file"<<endl;
        }
    ofile.close();
}

int main() {

    readfromDEF();
    readfromLEF();
    write("D://College//Semester 10-- Spring 2019//CSCE3304 - Digital Design II//Assignments//pro2//untitled//wav.spef");
   // cout << calculateSheetResistance(stof(find_layer(nets[0].connection[0].layer)),
          //  cout<<calculateSegmentLength(
    nets_size=5000;
   /* for (int j = 0; j < nets_size; j++) {
        cout<<nets[j].name<<endl;
    }
    for (int i=0;i<pins.size();i++)
    {
        cout<<pins[i].name<<"\t"<<pins[i].pin_state<<"\t"<<pins[i].firstcoordinate<<"\t"<<pins[i].secondcoordinate<<endl;
    }*/
    /*
    for (int j = 0; j < 10; j++) {
        cout<<"Net: "<<endl;
        cout<<nets[j].name<<endl;
        cout<<"conn: "<<endl;
        int r= nets[j].connection.size();
        cout<<r<<endl;
        for (int i=0;i<r;i++)
        {
            cout<< nets[j].connection[i].layer<<endl;
            cout<<"cord: "<<endl;
            int p= nets[j].connection[i].cord.size();
            for (int k=0;k<p;k++)
            {
                cout <<nets[j].connection[i].cord[k].x<<
                      nets[j].connection[i].cord[k].y<< endl;
            }
        }
    }*/
/*
    int x=stoi(nets[0].connection[0].cord[10].x);
    int y=stoi(nets[0].connection[0].cord[10].y);
    int x1=stoi(nets[0].connection[0].cord[11].x);
    int y1=y;
    float z=calculateSegmentLength(x,x1,y,y1);
    cout<<z<<endl;
    cout<<z<<"  "<<stof(layers[0].width)<<" "<<z/stof(layers[0].width)<<endl;
    cout<<calculateSegmentResistance(stof(layers[0].resistance_value),z,stof(layers[0].width))<<endl;
    cout<<calculateSegmentCapacitance(stof(layers[0].width),stof(layers[0].spacing),)
   // int y1=stoi(nets[0].connection[0].cord[11].y);
                  cout<< x <<"  "<<y  <<endl;
                  cout<< x1 <<"  "<<nets[0].connection[0].cord[11].y  <<endl;
*/
                          //, stoi(nets[0].connection[0].cord[1].x), stoi(nets[0].connection[0].cord[0].y), stoi(nets[0].connection[0].cord[1].y))<<endl;
            //, stoi(find_layer(nets[0].connection[0].layer)) << endl;
/*
    cout << "*CAP" << endl;
        for (int j = 0; j < nets_size; j++) {
            for (int k = 0; k < nets[j].connection.size()-1; k++) {// per segment
                string w= find_layerW(nets[j].connection[k].layer);
                string c=find_layerC(nets[j].connection[k].layer);
                string e =find_layerE(nets[j].connection[k].layer);
                for (int i=0;i<nets[j].connection[k].cord.size();i++){
                    if (i+1>=nets[j].connection[k].cord.size()) break;
                    if (nets[j].connection[k].cord[i+1].y=="*")
                        nets[j].connection[k].cord[i+1].y=nets[j].connection[k].cord[i].y;
                    if (nets[j].connection[k].cord[i+1].x=="*")
                        nets[j].connection[k].cord[i+1].x=nets[j].connection[k].cord[i].x;
                    if (nets[j].connection[k].cord[i].y=="*")
                        nets[j].connection[k].cord[i].y=nets[j].connection[k].cord[i+1].y;
                    if (nets[j].connection[k].cord[i].x=="*")
                        nets[j].connection[k].cord[i].x=nets[j].connection[k].cord[i+1].x;
                cout << i + 1 <<" "<< calculateSegmentCapacitance(stof(w), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)),  stof(c), stof(e)) << endl;
                segmentscapacitances.push_back(calculateSegmentCapacitance(stof(w), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)),  stof(c), stof(e)));
                }
        }
}
    cout << "*RES" << endl;
        for (int j = 0; j < nets_size; j++) {//per net
            for (int k = 0; k < nets[j].connection.size()-1; k++) {// per segment
               string w= find_layerW(nets[j].connection[k].layer);
               string r=find_layerR(nets[j].connection[k].layer);
               for (int i=0;i<nets[j].connection[k].cord.size();i++){
                   if (i+1>=nets[j].connection[k].cord.size()) break;
                   if (nets[j].connection[k].cord[i+1].y=="*")
                       nets[j].connection[k].cord[i+1].y=nets[j].connection[k].cord[i].y;
                   if (nets[j].connection[k].cord[i+1].x=="*")
                       nets[j].connection[k].cord[i+1].x=nets[j].connection[k].cord[i].x;
                   if (nets[j].connection[k].cord[i].y=="*")
                       nets[j].connection[k].cord[i].y=nets[j].connection[k].cord[i+1].y;
                   if (nets[j].connection[k].cord[i].x=="*")
                       nets[j].connection[k].cord[i].x=nets[j].connection[k].cord[i+1].x;
                cout <<nets[j].name << "  " << calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)) << endl;
                segmentsresistances.push_back(calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)));
               }}
        }
//vias
        for(int j=0;j<via.size();j++){
        cout << calculateViaResistance(stof(via[j].resistance)) << endl;
    }
*/
    return 0;
}
