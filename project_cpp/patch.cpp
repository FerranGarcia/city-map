#include "patch.h"

Patch::Patch(){}

void Patch::calcPatch(vector<Node*> path){
    bool bel = true;
    vector<string> route;
    char *right = "Turn right and go ahead";
    char *left = "Turn left and go ahead";
    char *end = "You have just arrived!";

    Node* init = path[0];
    float dist = 0;

    //Until the path finishes
    for(unsigned int i=1; i<path.size()-1; i++){
        //Compare if the couple of points belong to the same road
        bel = this->belong(path[i-1], path[i]);
        //If two points does not belong to the same road means that it is a cross where we have to turn
        if(bel == false){

            dist = init->distNode(path[i]);
            init = path[i];

            float angle= this->checkAngle(path[i], path[i-1] ,path[i+1]);

            if(angle>=30 && angle<160){
                cout<<"Go ahead "<<dist<<" meters and turn right: "<<angle<<" degrees"<<endl;
                route.push_back(right);
            }
            if(angle<-10 && angle>-160){
                cout<<"Go ahead "<<dist<<" meters and turn left: "<<angle<<" degrees"<<endl;
                route.push_back(left);
            }
        }
    }
    route.push_back(end);
    this->genTxt(route);
}

//Returns the angle between two segments defined by the same initial point "Center"
float Patch::checkAngle(Node* M, Node* P, Node* O){

    //Inspirated in http://stackoverflow.com/questions/3486172/angle-between-3-points
    POINTFLOAT ab = { M->getPoint().x - O->getPoint().x, M->getPoint().y - O->getPoint().y };
    POINTFLOAT cb = { M->getPoint().x - P->getPoint().x, M->getPoint().y - P->getPoint().y };

    float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
    float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

    float alpha = atan2(cross, dot);

    return (int) floor(alpha * 180. / 3.14159265359 + 0.5);

}

//Compares if two nodes belong to the same road
bool Patch::belong(Node* node1, Node* node2){
    string road1;
    string road2;

    int id1 = node1->getId();
    int id2 = node2->getId();

    QSqlQuery belongId1;
    QSqlQuery belongId2;

    belongId1.prepare("select roadID from road_node, node where road_node.NodeID = node.ID and node.ind= :id");
    belongId1.bindValue(":id", id1);
    belongId1.exec();

    belongId2.prepare("select roadID from road_node, node where road_node.NodeID = node.ID and node.ind= :id");
    belongId2.bindValue(":id", id2);
    belongId2.exec();

    //Take into account that the node can belong to more than one road
    for(unsigned int i=0; i<belongId1.size(); i++){
        belongId1.next();
        road1=belongId1.value(0).toString().toStdString();
        //cout<<id1<<" "<<road1<<endl;
        for(unsigned int j=0; j<belongId2.size()-1; j++){
            belongId2.next();
            road2 = belongId2.value(0).toString().toStdString();
            if(road1 == road2){
                return true;
            }
        }
        belongId2.first();
    }
    return false;
}

//Generates a txt file with name route.txt with all previous instructions
//The file is deleted overwrited each time the txt is generated in the compile folder
void Patch::genTxt(vector<string> route){

    ofstream file;
    file.open("route.txt");
    for(int i=0; i<route.size(); i++){
        file<<route[i]<<endl;
    }

    file.close();
}

//This function calculates the distance between the current position and the point
//  we have a change of orientation in the route
