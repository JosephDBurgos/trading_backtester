#include "schema/data_loader.h"

namespace bt {

std::vector<PriceBar> DataLoader::loadBars(const std::string& symbol,
                                           const std::string& timeframe) {
    // Future: add caching, symbol normalization, filtering, etc.
    return source_->load(symbol, timeframe);
}

} // namespace bt

