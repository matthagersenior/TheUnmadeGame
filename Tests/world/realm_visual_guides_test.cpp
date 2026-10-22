#include "World/UnmadeProceduralArtRules.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <set>
#include <string>
using namespace UnmadeCore;

int main(){
    std::set<std::string> names,motifs,colors;
    std::set<int> crowns;
    std::set<std::string> orientations;
    for(int i=0;i<9;++i){
        const auto& design=RealmVisualDesigns[i];
        assert(design.name && design.landmark && design.storyMotif);
        assert(names.insert(design.landmark).second);
        assert(motifs.insert(design.storyMotif).second);
        assert(crowns.insert(static_cast<int>(design.crown)).second);
        std::string orientation;
        for(const auto& rotation:RealmVisualRotations[i]){
            assert(rotation.pitch>=-20&&rotation.pitch<=20);
            assert(rotation.yaw>=-20&&rotation.yaw<=20);
            assert(rotation.roll>=-20&&rotation.roll<=20);
            orientation+=std::to_string(rotation.pitch)+":"+
                         std::to_string(rotation.yaw)+":"+
                         std::to_string(rotation.roll)+";";
        }
        assert(orientations.insert(orientation).second);
        assert(colors.insert(std::to_string(design.color.R)+":"+
                             std::to_string(design.color.G)+":"+
                             std::to_string(design.color.B)).second);
        for(const auto& part:design.silhouette){
            assert(part.dx<-1200 && part.dy>0 && part.dz>0);
            assert(part.sx>0 && part.sy>0 && part.sz>0);
        }
        char marker[70];
        std::snprintf(marker,sizeof(marker),"Expedition.%d.Landmark.0",i);
        auto landmark=ClassifyRealmProp(marker,1,1,3);
        assert(landmark.realm==i);
        assert(landmark.role==VisualRole::Landmark);
        assert(landmark.allowDecoration && landmark.keepCubeCollision);
        std::snprintf(marker,sizeof(marker),"Expedition.%d.Structure",i);
        auto road=ClassifyRealmProp(marker,100,170,.5);
        assert(road.realm==i && road.role==VisualRole::Foundation);
        assert(!road.allowDecoration && road.keepCubeCollision);
    }
    assert(crowns.size()==9 && names.size()==9 && motifs.size()==9 && orientations.size()==9);
    assert(ClassifyRealmProp("Expedition.99.Landmark",1,1,1).realm==-1);
    assert(ClassifyRealmProp("Expedition.2x.Landmark",1,1,1).realm==-1);
    assert(ClassifyRealmProp("",1,1,1).realm==-1);
    assert(ClassifyRealmProp("Later.9.Trial.Barrier",1,1,2).realm==-1);
    std::cout<<"PASS: nine distinct authored frontier silhouettes; original structure and collision authority preserved\n";
}
