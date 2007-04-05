#include "RecoTracker/TrackProducer/interface/TrackingRecHitLessFromGlobalPosition.h"

#include "Utilities/General/interface/CMSexception.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

bool TrackingRecHitLessFromGlobalPosition::insideOutLess(  const TrackingRecHit& a, const TrackingRecHit& b) const{
  
  DetId ida(a.geographicalId());
  DetId idb(b.geographicalId());

  unsigned int idetA = static_cast<unsigned int>(ida.subdetId());
  unsigned int idetB = static_cast<unsigned int>(idb.subdetId());

  //check for mixed case...
  bool same_det =
    (
     (idetA == StripSubdetector::TIB         && idetB == StripSubdetector::TID) ||
     (idetA == StripSubdetector::TID         && idetB == StripSubdetector::TIB) ||
     (idetA == StripSubdetector::TOB         && idetB == StripSubdetector::TEC) ||
     (idetA == StripSubdetector::TEC         && idetB == StripSubdetector::TOB) ||
     (idetA == PixelSubdetector::PixelBarrel && idetB == PixelSubdetector::PixelEndcap) ||
     (idetA == PixelSubdetector::PixelEndcap && idetB == PixelSubdetector::PixelBarrel));

  if (!same_det) return (idetA < idetB);


  if( (static_cast<unsigned int>(ida.subdetId()) == StripSubdetector::TIB || static_cast<unsigned int>(ida.subdetId()) == StripSubdetector::TOB || static_cast<unsigned int>(ida.subdetId()) == PixelSubdetector::PixelBarrel) &&
      (static_cast<unsigned int>(idb.subdetId()) == StripSubdetector::TIB || static_cast<unsigned int>(idb.subdetId()) == StripSubdetector::TOB || static_cast<unsigned int>(idb.subdetId()) == PixelSubdetector::PixelBarrel)) {  // barrel with barrel
    //float diff = geometry->idToDet(ida)->surface().toGlobal(a.localPosition()).perp() - geometry->idToDet(idb)->surface().toGlobal(b.localPosition()).perp();

  //if (std::abs(diff)<1.0e-9) return false;
    return  geometry->idToDet(ida)->surface().toGlobal(a.localPosition()).perp()< geometry->idToDet(idb)->surface().toGlobal(b.localPosition()).perp();
  }
  
  if( (static_cast<unsigned int>(ida.subdetId()) == StripSubdetector::TEC || static_cast<unsigned int>(ida.subdetId()) == StripSubdetector::TID || static_cast<unsigned int>(ida.subdetId()) == PixelSubdetector::PixelEndcap) &&
      (static_cast<unsigned int>(idb.subdetId()) == StripSubdetector::TEC || static_cast<unsigned int>(idb.subdetId()) == StripSubdetector::TID || static_cast<unsigned int>(idb.subdetId()) == PixelSubdetector::PixelEndcap)) {  // fwd with fwd
    //float diff = std::abs( geometry->idToDet(ida)->surface().toGlobal(a.localPosition()).z()) - 
    //             std::abs( geometry->idToDet(idb)->surface().toGlobal(b.localPosition()).z());

    //if (std::abs(diff)<1.0e-9) return false;
    //else return (diff < 0);
    return std::abs( geometry->idToDet(ida)->surface().toGlobal(a.localPosition()).z()) < std::abs( geometry->idToDet(idb)->surface().toGlobal(b.localPosition()).z());
  }
  
  //
  //  here I have 1 barrel against one forward
  //
  
  if( (static_cast<unsigned int>(ida.subdetId()) == StripSubdetector::TIB || static_cast<unsigned int>(ida.subdetId()) == StripSubdetector::TOB || static_cast<unsigned int>(ida.subdetId()) == PixelSubdetector::PixelBarrel) &&
      (static_cast<unsigned int>(idb.subdetId()) == StripSubdetector::TEC || static_cast<unsigned int>(idb.subdetId()) == StripSubdetector::TID || static_cast<unsigned int>(idb.subdetId()) == PixelSubdetector::PixelEndcap)) {  // barrel with barrel
    return barrelForwardLess( a, b);
  }else{
    return !barrelForwardLess( b, a);
  }
  
  //throw DetLogicError("GeomDetLess: arguments are not Barrel or Forward GeomDets");
  throw Genexception("TrackingRecHitLessFromGlobalPosition: arguments are not Ok");
}

bool TrackingRecHitLessFromGlobalPosition::barrelForwardLess(  const TrackingRecHit& a, const TrackingRecHit& b) const{
  //
  // for the moment sort again in z, but since the z in the barrel is wrong (it is in the centre of the module)
  // add the semi length
  //
  DetId ida(a.geographicalId());
  DetId idb(b.geographicalId());
  BoundPlane s =  geometry->idToDet(ida)->specificSurface();
  // const Bounds * bb     = &(s.bounds());

  // float diff = std::abs( geometry->idToDet(ida)->surface().toGlobal(a.localPosition()).z())+ std::abs(bb->length()/2.) -
  //  std::abs( geometry->idToDet(idb)->surface().toGlobal(b.localPosition()).z());

  //if (std::abs(diff)<1.0e-9) return false;
  //else return (diff < 0);
  return  std::abs( geometry->idToDet(ida)->surface().toGlobal(a.localPosition()).z()) < std::abs( geometry->idToDet(idb)->surface().toGlobal(b.localPosition()).z());
}
