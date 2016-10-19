// $Id$
//==========================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
#ifndef DD4HEP_GEOMETRY_SEGMENTATIONS_H
#define DD4HEP_GEOMETRY_SEGMENTATIONS_H

// Framework include files
#include "DD4hep/Objects.h"
#include "DD4hep/Handle.h"
#include "DDSegmentation/Segmentation.h"

// C/C++ include files
#include <cmath>
#include <vector>

/// Namespace for the AIDA detector description toolkit
namespace DD4hep {

  /// Namespace for the geometry part of the AIDA detector description toolkit
  namespace Geometry {

    // Forward declarations
    class DetElementObject;
    class SensitiveDetectorObject;

    /// Implementation class supporting generic Segmentation of sensitive detectors
    /**
     *  The SegmentationObject wraps the functionality of the DDSegmentation base class.
     *
     *  \author  M.Frank
     *  \version 1.0
     *  \ingroup DD4HEP_GEOMETRY
     */
    class SegmentationObject {
    public:
      typedef DDSegmentation::Segmentation BaseSegmentation;
      typedef DDSegmentation::Parameters   Parameters;
      typedef DDSegmentation::Parameter    Parameter;

    public:
      /// Standard constructor
      SegmentationObject(BaseSegmentation* s = 0);
      /// Default destructor
      virtual ~SegmentationObject();
      /// Access the encoding string
      std::string fieldDescription() const;
      /// Access the segmentation name
      const std::string& name() const;
      /// Set the segmentation name
      void setName(const std::string& value);
      /// Access the segmentation type
      const std::string& type() const;
      /// Access the description of the segmentation
      const std::string& description() const;
      /// Access the underlying decoder
      BitField64* decoder() const;
      /// Set the underlying decoder
      void setDecoder(BitField64* decoder) const;
      /// Access to parameter by name
      Parameter  parameter(const std::string& parameterName) const;
      /// Access to all parameters
      Parameters parameters() const;
      /// Set all parameters from an existing set of parameters
      void setParameters(const Parameters& parameters);

      /** Segmentation interface  */
      /// Determine the local position based on the cell ID
      Position position(const CellID& cellID) const;
      /// Determine the cell ID based on the position
      CellID cellID(const Position& localPosition,
                    const Position& globalPosition,
                    const VolumeID& volumeID) const;
      /// Determine the volume ID from the full cell ID by removing all local fields
      VolumeID volumeID(const CellID& cellID) const;
      /// Calculates the neighbours of the given cell ID and adds them to the list of neighbours
      void neighbours(const CellID& cellID, std::set<CellID>& neighbours) const;

      /// Magic word to check object integrity
      unsigned long magic;
      /// Flag to use segmentation for hit positioning
      unsigned char useForHitPosition;
      /// Reference to hosting top level DetElement structure
      Handle<DetElementObject> detector;      
      /// Reference to hosting top level sensitve detector structure
      Handle<SensitiveDetectorObject> sensitive;
      /// Reference to base segmentation
      BaseSegmentation* segmentation;      
    };

    /// Concrete wrapper class for segmentation implementation based on DDSegmentation objects
    /**
     * \author  M.Frank
     * \version 1.0
     * \ingroup DD4HEP_GEOMETRY
     */
    template <typename IMP> class SegmentationWrapper : public SegmentationObject {
    public:
      /// DDSegmentation aggregate
      IMP* implementation;
    public:
      /// Standard constructor
      SegmentationWrapper(DDSegmentation::BitField64* decoder);
      /// Default destructor
      virtual ~SegmentationWrapper();
    };

    /// Standard constructor
    template <typename IMP> inline
    SegmentationWrapper<IMP>::SegmentationWrapper(DDSegmentation::BitField64* decode)
      :  SegmentationObject(implementation=new IMP(decode))
    {
    }
    
    /// Default destructor
    template <typename IMP> inline SegmentationWrapper<IMP>::~SegmentationWrapper()  {
    }
    
    /// Handle class supporting generic Segmentation of sensitive detectors
    /**
     *
     *   \author  M.Frank
     *   \version 1.0
     *   \ingroup DD4HEP_GEOMETRY
     */
    class Segmentation: public Handle<SegmentationObject> {
    public:
      typedef SegmentationObject Object;
      typedef DDSegmentation::Segmentation BaseSegmentation;
      typedef DDSegmentation::Parameter    Parameter;
      typedef DDSegmentation::Parameters   Parameters;

    public:
      /// Initializing constructor creating a new object of the given DDSegmentation type
      Segmentation(const std::string& type, const std::string& name, BitField64* decoder);
      /// Default constructor
      Segmentation() : Handle<Object>() { }
      /// Copy Constructor from object
      Segmentation(const Segmentation& e) : Handle<Object>(e) { }
#ifndef __CINT__
      /// Copy Constructor from handle
      Segmentation(const Handle<Object>& e) : Handle<Object>(e) { }
#endif
      /// Constructor to be used when reading the already parsed object
      template <typename Q>
      Segmentation(const Handle<Q>& e) : Handle<Object>(e) { }
      /// Assignment operator
      Segmentation& operator=(const Segmentation& seg) = default;
      /// Access flag for hit positioning
      bool useForHitPosition() const;
      /// Accessor: Segmentation type
      std::string type() const;
      /// Access to the parameters
      Parameters parameters() const;
      /// Access the underlying decoder
      BitField64* decoder() const;
      /// Set the underlying decoder
      void setDecoder(BitField64* decoder) const;
      /// determine the local position based on the cell ID
      Position position(const long64& cellID) const;
      /// determine the cell ID based on the local position
      long64 cellID(const Position& localPosition, const Position& globalPosition, const long64& volumeID) const;
      /// Access the concrete underlying segmentation implementation from DDSegmentation
      template <typename T> static T* get(const Object* object);
    };

  } /* End namespace Geometry              */
} /* End namespace DD4hep                */
#endif    /* DD4HEP_GEOMETRY_SEGMENTATIONS_H     */
