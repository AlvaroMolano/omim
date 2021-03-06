#pragma once

#include "drape_frontend/custom_symbol.hpp"
#include "drape_frontend/map_shape.hpp"
#include "drape_frontend/metaline_manager.hpp"
#include "drape_frontend/tile_key.hpp"
#include "drape_frontend/traffic_generator.hpp"

#include "drape/pointers.hpp"

#include "indexer/road_shields_parser.hpp"

#include "geometry/rect2d.hpp"
#include "geometry/screenbase.hpp"

#include <array>
#include <functional>
#include <map>
#include <string>
#include <unordered_set>

class FeatureType;

namespace df
{
class EngineContext;
class Stylist;

class RuleDrawer
{
public:
  using TDrawerCallback = std::function<void(FeatureType const &, Stylist &)>;
  using TCheckCancelledCallback = std::function<bool()>;
  using TIsCountryLoadedByNameFn = std::function<bool(std::string const &)>;
  using TInsertShapeFn = function<void(drape_ptr<MapShape> && shape)>;

  RuleDrawer(TDrawerCallback const & drawerFn,
             TCheckCancelledCallback const & checkCancelled,
             TIsCountryLoadedByNameFn const & isLoadedFn,
             ref_ptr<EngineContext> engineContext);
  ~RuleDrawer();

  void operator()(FeatureType const & f);

private:
  void ProcessAreaStyle(FeatureType const & f, Stylist const & s, TInsertShapeFn const & insertShape,
                        int & minVisibleScale);
  void ProcessLineStyle(FeatureType const & f, Stylist const & s, TInsertShapeFn const & insertShape,
                        int & minVisibleScale);
  void ProcessPointStyle(FeatureType const & f, Stylist const & s, TInsertShapeFn const & insertShape,
                         int & minVisibleScale);

  bool CheckCoastlines(FeatureType const & f, Stylist const & s);

#ifdef DRAW_TILE_NET
  void DrawTileNet(TInsertShapeFn const & insertShape);
#endif

  bool CheckCancelled();

  TDrawerCallback m_callback;
  TCheckCancelledCallback m_checkCancelled;
  TIsCountryLoadedByNameFn m_isLoadedFn;

  ref_ptr<EngineContext> m_context;
  CustomSymbolsContextPtr m_customSymbolsContext;
  std::unordered_set<m2::Spline const *> m_usedMetalines;

  m2::RectD m_globalRect;
  double m_currentScaleGtoP;
  double m_trafficScalePtoG;

  TrafficSegmentsGeometry m_trafficGeometry;

  std::array<TMapShapes, df::MapShapeTypeCount> m_mapShapes;
  bool m_wasCancelled;

  GeneratedRoadShields m_generatedRoadShields;
};
}  // namespace df
