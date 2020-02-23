Ts2dInstanceType(line,              sizeof(f32) * 8,  TS2D_MAX_LINES)
Ts2dInstanceType(rect,              sizeof(f32) * 8,  TS2D_MAX_RECTS)
Ts2dInstanceType(filled_rect,       sizeof(f32) * 20, TS2D_MAX_FILLED_RECTS)
Ts2dInstanceType(texture,           sizeof(f32) * 12, TS2D_MAX_TEXTURES)
Ts2dInstanceType(world_tile,        sizeof(u8) * 4,   TS2D_MAX_WORLD_TILES)
Ts2dInstanceType(text_char,         sizeof(f32) * 16, TS2D_MAX_TEXT_CHARS)
Ts2dInstanceType(reflective_rect,   sizeof(f32) * 12, TS2D_MAX_REFLECTIVE_RECTS)
#undef Ts2dInstanceType