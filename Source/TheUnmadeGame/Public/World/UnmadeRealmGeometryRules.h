#pragma once
// Portable layout contract for the generated six-realm walkable prototype.
// Unreal's BasicShapes/Cube has 100-centimeter dimensions, i.e. half 50 cm.
namespace UnmadeCore {
struct Rect2 {
    double minX,maxX,minY,maxY;
};
inline constexpr bool Overlaps(Rect2 a,Rect2 b) noexcept {
    return a.minX<b.maxX && a.maxX>b.minX && a.minY<b.maxY && a.maxY>b.minY;
}
inline constexpr bool Contains(Rect2 r,double x,double y) noexcept {
    return x>=r.minX && x<=r.maxX && y>=r.minY && y<=r.maxY;
}
struct LaterArenaLayout {
    static constexpr double SouthCenterY=-800,SouthHalfY=1500;
    static constexpr double NorthCenterY=2450,NorthHalfY=1300;
    static constexpr double BridgeCenterY=970,BridgeHalfY=600;
    static constexpr double FloorHalfX=2600;
    static constexpr Rect2 South() noexcept {
        return {-FloorHalfX,FloorHalfX,SouthCenterY-SouthHalfY,
                SouthCenterY+SouthHalfY};
    }
    static constexpr Rect2 North() noexcept {
        return {-FloorHalfX,FloorHalfX,NorthCenterY-NorthHalfY,
                NorthCenterY+NorthHalfY};
    }
    static constexpr Rect2 BridgeCare() noexcept {
        return {-1250,-450,BridgeCenterY-BridgeHalfY,BridgeCenterY+BridgeHalfY};
    }
    static constexpr Rect2 BridgeTruth() noexcept {
        return {450,1250,BridgeCenterY-BridgeHalfY,BridgeCenterY+BridgeHalfY};
    }
    static constexpr Rect2 WallLeft() noexcept {return {-2600,-1250,2102.5,2157.5};}
    static constexpr Rect2 WallMiddle() noexcept {return {-450,450,2102.5,2157.5};}
    static constexpr Rect2 WallRight() noexcept {return {1250,2600,2102.5,2157.5};}
    static constexpr Rect2 GateCare() noexcept {return {-1250,-450,2102.5,2157.5};}
    static constexpr Rect2 GateTruth() noexcept {return {450,1250,2102.5,2157.5};}
    static constexpr Rect2 ResonanceSpan() noexcept {
        return {-400,400,3500,4200};
    }
    static constexpr Rect2 ResonanceIsland() noexcept {
        return {-700,700,4050,4550};
    }
    static constexpr bool Sound() noexcept {
        return Overlaps(North(),ResonanceSpan()) &&
            Overlaps(ResonanceSpan(),ResonanceIsland()) &&
            !Overlaps(North(),ResonanceIsland()) &&
            Contains(ResonanceIsland(),0,4380) &&
            !Overlaps(South(),North()) &&
            Overlaps(South(),BridgeCare()) && Overlaps(North(),BridgeCare()) &&
            Overlaps(South(),BridgeTruth()) && Overlaps(North(),BridgeTruth()) &&
            !Overlaps(BridgeCare(),BridgeTruth()) &&
            !Overlaps(WallLeft(),GateCare()) &&
            !Overlaps(WallMiddle(),GateCare()) &&
            !Overlaps(WallMiddle(),GateTruth()) &&
            !Overlaps(WallRight(),GateTruth()) &&
            Contains(South(),0,-1170) &&
            Contains(South(),0,-180) &&
            Contains(North(),1500,1800) &&
            Contains(North(),-850,1940) &&
            Contains(North(),850,1940) &&
            Contains(North(),0,3000);
    }
};
static_assert(LaterArenaLayout::Sound(),"Later realms need a real gap and usable gates");
} // namespace UnmadeCore
