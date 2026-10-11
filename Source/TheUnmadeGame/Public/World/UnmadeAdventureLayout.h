#pragma once
#include <array>
namespace UnmadeCore {
struct DungeonPoint {int x,y;};
inline DungeonPoint DungeonRoom(int arc,int room){constexpr int paths[9][6][2]={
 {{0,0},{1,0},{1,1},{0,1},{0,2},{1,2}},{{0,0},{0,1},{1,1},{1,2},{2,2},{2,3}},
 {{0,0},{1,0},{2,0},{2,1},{1,1},{1,2}},{{0,0},{0,1},{0,2},{1,2},{1,1},{2,1}},
 {{0,0},{1,0},{1,1},{2,1},{2,2},{3,2}},{{0,0},{0,1},{1,1},{2,1},{2,2},{2,3}},
 {{0,0},{-1,0},{-1,1},{-2,1},{-2,2},{-1,2}},{{0,0},{1,0},{1,-1},{2,-1},{2,0},{2,1}},
 {{0,0},{-1,0},{-2,0},{-2,1},{-1,1},{-1,2}}};
 if(arc<0||arc>8||room<0||room>5)return {0,0};
 return {paths[arc][room][0]*2600,paths[arc][room][1]*2600};}
inline bool DungeonDoor(int arc,int room,int dx,int dy){auto p=DungeonRoom(arc,room);if(room==0&&dx==0&&dy==-1)return true;if(room==5&&dx==0&&dy==1)return true;for(int n:{room-1,room+1})if(n>=0&&n<6){auto q=DungeonRoom(arc,n);if(q.x-p.x==dx*2600&&q.y-p.y==dy*2600)return true;}return false;}
inline int EnemyRoleAt(int arc,int slot){constexpr int roles[9][8]={{0,1,2,3,0,1,2,3},{2,3,1,2,0,3,2,1},{0,3,0,1,0,0,3,1},{3,0,2,1,0,3,0,2},{2,2,3,1,0,2,3,2},{1,0,1,3,0,1,0,3},{2,1,3,2,0,2,1,3},{0,1,0,3,0,1,1,0},{3,2,3,0,0,3,2,0}};return arc>=0&&arc<9&&slot>=0&&slot<8?roles[arc][slot]:0;}
}
