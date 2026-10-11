#pragma once
namespace UnmadeCore {
struct IdentitySpec{const char* mechanism;const char* hint;const char* ambience;};
inline constexpr IdentitySpec IdentitySpecs[]={
 {"Charter Signal","Light every road signal","crossings"},
 {"Breakwater Pumps","Drain outer pumps; keep middle dark","saltwake"},
 {"Kiln Pressure","Open two intake valves; close exhaust","cinderhold"},
 {"Oath Buoys","Float the two downstream buoys","drevlach"},
 {"Choir Counterweights","Ground only the first counterweight","orravane"},
 {"Yesterday Seams","Expose only the oldest seam","vathless"},
 {"Census Lamps","Leave only the unlisted lamp lit","eillun"},
 {"Root Grafts","Restore all three grafts","tharniv"},
 {"Threshold Lamps","Light the two boundary lamps","auvren"},
};
}
