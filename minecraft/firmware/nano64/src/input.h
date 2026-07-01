#pragma once

enum class Key {
    Left,
    Right,
    Up,
    Down,
    Break,
    Place,
    Cycle
};

class Input {
public:
    void poll();
    bool isKeyDown(Key k) const;
};
