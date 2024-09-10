#include "base/mapscr.h"
#include "base/general.h"
#include "base/zsys.h"
#include "base/qrs.h"

std::vector<mapscr> TheMaps;
std::vector<word> map_autolayers;
word map_count = 0;

byte mapscr::ffEffectWidth(size_t ind) const
{
	return (byte)ffcs[ind].hit_width;
}

void mapscr::ffEffectWidth(size_t ind, byte val)
{
	ffcs[ind].hit_width = val;
}

byte mapscr::ffEffectHeight(size_t ind) const
{
	return ffcs[ind].hit_height;
}

void mapscr::ffEffectHeight(size_t ind, byte val)
{
	ffcs[ind].hit_height = val;
}

byte mapscr::ffTileWidth(size_t ind) const
{
	return ffcs[ind].txsz;
}

void mapscr::ffTileWidth(size_t ind, byte val)
{
	ffcs[ind].txsz = val;
}

byte mapscr::ffTileHeight(size_t ind) const
{
	return ffcs[ind].tysz;
}

void mapscr::ffTileHeight(size_t ind, byte val)
{
	ffcs[ind].tysz = val;
}

void mapscr::zero_memory()
{
	*this = mapscr();
}

void mapscr::ensureFFC(size_t ind)
{
	getFFC(ind);
}

ffcdata& mapscr::getFFC(size_t ind)
{
	assert(ind < MAXFFCS);

	if (ind < ffcs.size())
		return ffcs[ind];

	// ffc_count_dirty does not need to be set because `data` is still zero, so the count
	// won't have changed. Only need to mark it dirty when `data`
	// changes (handled in `screen_ffc_modify_postroutine`).
	ffcs.resize(ind + 1);
	return ffcs[ind];
}

word mapscr::numFFC()
{
	if (!ffc_count_dirty)
		return num_ffcs;

	int lastffc = -1;
	for (int w = ffcs.size() - 1; w >= 0; --w)
	{
		if (ffcs[w].data)
		{
			lastffc = w;
			break;
		}
	}
	if (lastffc < 31 && get_qr(qr_OLD_FFC_FUNCTIONALITY))
	{
		lastffc = 31;
		ensureFFC(31);
	}
	num_ffcs = lastffc + 1;
	ffc_count_dirty = false;
	return num_ffcs;
}

void mapscr::ffcCountMarkDirty()
{
	ffc_count_dirty = true;
}
