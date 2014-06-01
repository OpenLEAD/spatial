#ifndef spatial_TYPES_HPP
#define spatial_TYPES_HPP

/* If you need to define types specific to your oroGen components, define them
 * here. Required headers must be included explicitly
 *
 * However, it is common that you will only import types from your library, in
 * which case you do not need this file
 */

#include <base/time.h>
#include <base/angle.h>

namespace spatial {

struct EulerAngles{
	base::Time time;
	base::Angle roll;
	base::Angle yaw;
	base::Angle pitch;
};

}

#endif

