$input v_color0

/*
 * Copyright 2011-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"
uniform vec4 u_color;

void main()
{
	gl_FragColor = v_color0;
    gl_FragColor = u_color;
}
