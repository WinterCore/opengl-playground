#version 460 core

layout (location = 0) in vec2 fragPos;

out vec4 FragColor;

void main() {
    float halfWidth = 0.5;
    float halfHeight = 0.5;

    // Calculate distance from edge
    float distLeft   = abs(fragPos.x + halfWidth);
    float distRight  = abs(fragPos.x - halfWidth);
    float distTop    = abs(fragPos.y - halfHeight);
    float distBottom = abs(fragPos.y + halfHeight);

    float minDistX = min(distLeft, distRight);
    float minDistY = min(distTop, distBottom);
    float minDist  = min(minDistX, minDistY);

    if (minDist < 0.1)
    {
        // Border color
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else
    {
        // Fill color
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
