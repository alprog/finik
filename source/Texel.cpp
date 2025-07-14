module Texel;

const Texel Texel::Transparent = Texel{0, 0, 0, 0};
const Texel Texel::Black = Texel{0, 0, 0, 0xFF};
const Texel Texel::White = Texel{0xFF, 0xFF, 0xFF, 0xFF};

const Texel Texel::Red = Texel{0xFF, 0, 0, 0xFF};
const Texel Texel::Green = Texel{0, 0xFF, 0, 0xFF};
const Texel Texel::Blue = Texel{0, 0, 0xFF, 0xFF};

const Texel Texel::Yellow = Texel{0xFF, 0xFF, 0, 0xFF};
const Texel Texel::Magenta = Texel{0xFF, 0, 0xFF, 0xFF};
const Texel Texel::Cyan = Texel{0, 0xFF, 0xFF, 0xFF};
