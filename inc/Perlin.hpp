#ifndef PERLIN_HPP
#define PERLIN_HPP
#include "common.hpp"

// Usage: PerlinNoise pn(seed, repeatX, repeatY); float n = pn.noise(x, y);

class PerlinNoise {
public:
    // repeatX, repeatY: periods after which noise wraps in x and y (must be > 0 and <= 256, typically powers of two)
    explicit PerlinNoise(unsigned int seed, int repeatX = 256, int repeatY = 256)
        : repeatX_(repeatX), repeatY_(repeatY)
    {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::default_random_engine engine(mt_gen());
        std::shuffle(p.begin(), p.end(), engine);
        p.insert(p.end(), p.begin(), p.end());
    }

    // 2D Perlin noise, output in [-1, 1]
    float noise(float x, float y) const {
        if (repeatX_ > 0) {
            x = fmodf(x, float(repeatX_));
            if (x < 0) x += repeatX_;
        }
        if (repeatY_ > 0) {
            y = fmodf(y, float(repeatY_));
            if (y < 0) y += repeatY_;
        }

        int xi = static_cast<int>(floorf(x)) % (int) repeatX_;
        int yi = static_cast<int>(floorf(y)) % (int) repeatY_;

        float xf = x - floorf(x);
        float yf = y - floorf(y);

        float u = fade(xf);
        float v = fade(yf);

        int aa = hash(xi    , yi    );
        int ab = hash(xi    , yi + 1);
        int ba = hash(xi + 1, yi    );
        int bb = hash(xi + 1, yi + 1);

        float x1 = lerp(grad(aa, xf    , yf    ), grad(ba, xf - 1, yf    ), u);
        float x2 = lerp(grad(ab, xf    , yf - 1), grad(bb, xf - 1, yf - 1), u);
        return lerp(x1, x2, v);
    }

    // Set new repeat periods
    void setRepeat(int repeatX, int repeatY) {
        repeatX_ = repeatX;
        repeatY_ = repeatY;
    }

private:
    std::vector<int> p;
    int repeatX_;
    int repeatY_;

    int hash(int x, int y) const {
        if (repeatX_ > 0) x = x % repeatX_;
        if (repeatY_ > 0) y = y % repeatY_;
        return p[p[x & 255] + (y & 255)];
    }

    static float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    static float grad(int hash, float x, float y) {
        int h = hash & 7;
        float u = h < 4 ? x : y;
        float v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
    }
};

class FractalNoise {
    public:
        FractalNoise(unsigned long seed, int nTheta, int nPhi,
                    double initial_freq, int octaves = 10, 
                     double lacunarity = 2.0, double scale0 = 0.5)
            : m_seed(seed), m_nTheta(nTheta), m_nPhi(nPhi),
              m_octaves(octaves), m_initial_freq(initial_freq),
              m_lacunarity(lacunarity), m_scale0(scale0) {
            
            std::mt19937 rng(m_seed);
            double freq = m_initial_freq;
            
            for (int i = 0; i < m_octaves; i++) {
                // Generate unique seed for each octave from main seed
                unsigned long octave_seed = rng();
                // Calculate periods for seamless tiling
                double periodX = m_nTheta * freq;
                double periodY = m_nPhi * freq;
                m_perlins.emplace_back(octave_seed, periodX, periodY);
                freq *= m_lacunarity;
            }
        }
    
        double noise(double x, double y) const {
            double result = 0.0;
            double amplitude = 1.0;
            double freq = m_initial_freq;
    
            for (const auto& p : m_perlins) {
                // Transform coordinates for current octave
                double tx = x * freq + 0.5;
                double ty = y * freq + 0.5;
                result += m_scale0 * p.noise(tx, ty) * amplitude;
                amplitude /= m_lacunarity;
                freq *= m_lacunarity;
            }
            return result;
        }
    
    private:
        unsigned long m_seed;
        int m_nTheta;
        int m_nPhi;
        int m_octaves;
        double m_initial_freq;
        double m_lacunarity;
        double m_scale0;
        std::vector<PerlinNoise> m_perlins;
    };

#endif // PERLIN_HPP
