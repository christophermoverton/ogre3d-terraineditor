/** @mainpage  dependices https://github.com/FFTW/fftw3  fast fourier transform libraries
  * @author Indrek Mandre indrek(at)mare.ee http://www.mare.ee/indrek/libfbm/
  *
  * This library generates (simulates) multi-dimensional random fields:
  *   * stationary Gaussian processes (SGP), with a user given covariance function
  *   * fractional Brownian motion (fBm)
  *   * random fields with power spectrum as power law 
  *
  * Stationary means here that the covariance between two points does not depend
  * on the individual coordinates of the points:
  *   \f[ \mbox{Cov}\!\left(Z\!\left(\mathbf{x}\right),Z\!\left(\mathbf{y}\right)\right)\equiv\rho\!\left(\mathbf{x}-\mathbf{y}\right), \f]
  * so that only the vector \f$x-y\f$ is used. Gaussian means that the distribution
  * of individual values in the field is gaussian and the process can be
  * completely described by its mean and covariance matrix, that is when
  *   \f[ \left[Z\!\left(\mathbf{x}_{1}\right),\ldots,Z\!\left(\mathbf{x}_{m}\right)\right]^{T}\sim\mathcal{N}\!\left(\boldsymbol{\mu},\boldsymbol{\Sigma}\right) \f]
  *
  * We say the SGP is isotropic, if the covariance depends only on the distance
  * between the two points, that is
  *   \f[ \mbox{Cov}\!\left(Z\!\left(\mathbf{x}\right),Z\!\left(\mathbf{y}\right)\right)\equiv\rho\!\left(\left|\mathbf{x}-\mathbf{y}\right|\right). \f]
  *
  * We say the SGP is even, if the covariance function has the same value
  * in case of reflection in one dimension, that is when:
  *   \f[ \mbox{Cov}\!\left(Z\!\left(\mathbf{x}\right),Z\!\left(\mathbf{y}\right)\right)\equiv\rho\!\left(\left|x_1-y_1\right|,\ldots,\left|x_d-y_d\right|\right). \f]
  *
  * Library supports arbitrary-dimensional generation, that is 1D, 2D, 3D, 4D, etc.
  * However, there is a hardcoded limit of 8 (can be changed at library compile time).
  *
  * For a random engine the library makes use of SFMT [8]. Normal distribution is achieved
  * with an efficient ziggurat algorithm (rejection sampling). Emphasis of the library
  * is on fast bulk-generation of random fields.
  *
  * With the help of some post-processing also fractional Brownian motion
  * or surfaces (or higher-dimensional fields) can be generated. For the
  * one-dimensional (1D) case this can be done simply by generating
  * the increment process (fractional Gaussian noise), and summing elements
  * to get the fBm itself. For higher-dimensional cases a more involved process
  * is used by Stein [3]. Note that Stein's definition of fBm is different compared
  * to the traditional (strict mathematical) definition -- fields generated with
  * his methods also adhere to this change.
  *
  * Calculation of a SGP involves two N-dimensional fast Fourier transforms.
  * One of them is pre-calculated and can be cached on the hard disk to save memory.
  * For this reason the first invocation of generate() on a given field object
  * may be at least twice as slow as the following calls (unless already cached on HD).
  * The library is meant to be used in Monte Carlo simulations where thousands or
  * millions of field instances are required.
  *
  * The first Fourier transform results in eigenvalues. For the algorithm
  * to work these must be positive. Sometimes, due to numeric errors or the
  * nature of cov() (the embedded covariance matrix is not positive semi-definite),
  * they can come out negative. When this happens the library complains to stderr.
  * Here are some hints on how to overcome this:
  * * from [2]: use of a nugget effect. Add a small value (say 1e-12) to Cov(0)
  *   to avoid numerical instabilities that result in negative eigenvalues.
  * * from [1] and [2]: increase field size (or reduce the scaling of
  *   the Cov() function) to make the matrix positive-semidefinite.
  * * from [1]: if there are only a few negative eigenvalues,
  *   the library sets them to 0 and scales the other values to compensate.
  *   However, I've not tested how well this works -- this is bound to
  *   introduce distortions.
  * * for the Stein's fBm method, increase the value of R.
  *   Note that this comes at the expense of increased memory usage and
  *   computation time. Conversely, reducing R to minimum that "works"
  *   will result in reduced memory usage and computational expense.
  * * Stein's fBm method [3] uses molding of the isotropic covariance function
  *   at edges to make it positive semi-definite. At the expense of losing some space
  *   at the edges one can still embed a given isotropic covariance function and save a lot
  *   of computational time as opposed to idea in tip 2 (simply increasing
  *   the field size which sometimes is not feasible). See [3] for more details.
  * * use a highly-composite or power-of-two for field sizes
  *   as the FFT functions are more efficient with these (both space and time).
  * * use common sense, test stuff out, fiddle with parameters.
  *   This library is no silver bullet. Make sure what you get is what you wanted.
  *
  * The used methods in this library are derived from [1], [2] and [3].
  * Also [5] contains useful information and implementation for matlab/octave.
  * Note that these methods are mathematically "exact". That is they are not
  * approximations. Still I should add that here by default we use
  * a pseudo-random random number generataror, which is not "exact".
  * And there are numeric errors that may creep up whenever calculations
  * approach zero or infinity.
  *
  * The library depends on the FFTW3 library for FFT functions.
  * You can get it from http://www.fftw.org/ but most Linux distribution have
  * a native package for easy installation.
  *
  * The library requires that the underlyinf architecture uses
  * IEEE 754 format double precision floating point numbers.
  *
  * I haven't really tested the library that much for non-isotropic or
  * un-even covariance functions. Hopefully, it works properly. If you
  * run into trouble you can use SGPTester to test that the generated
  * process has the expected covariances.
  *
  * <pre>
  * References:
  * [1] Wood, A. T. A. and Chan, G.
  *     Simulation of Stationary Gaussian Processes in [0, 1]^d.
  *     Journal of Computational and Graphical Statistics, 3(4), 409–432 (1994).
  *     doi:10.1080/10618600.1994.10474655
  * [2] Dietrich, C. R. and Newsam, G. N.
  *     Fast and Exact Simulation of Stationary Gaussian Processes through
  *     Circulant Embedding of the Covariance Matrix.
  *     SIAM Journal on Scientific Computing, 18(4), 1088–1107 (1997).
  *     doi:10.1137/S1064827592240555
  * [3] Stein, M. L.
  *     Fast and Exact Simulation of Fractional Brownian Surfaces.
  *     Journal of Computational and Graphical Statistics, 11(3), 587–599 (2002).
  *     doi:10.1198/106186002466
  * [4] Qian, H., Raymond, G. M. and Bassingthwaighte, J. B.
  *     On two-dimensional fractional Brownian motion and fractional Brownian random field.
  *     J. Phys. A: Math. Gen., 31, L527–L535 (1998).
  *     doi:10.1088/0305-4470/31/28/002
  * [5] Kroese, D. P. and Botev, I. Z.
  *     Spatial Process Generation.
  *     http://www.maths.uq.edu.au/~kroese/ps/MCSpatial.pdf
  * [6] Timmer, J. and König, M.
  *     On generating power law noise.
  *     Astronomy and Astrophysics 300: 707 (1995).
  * [7] Marsaglia, G. and Tsang, W. W.
  *     The Ziggurat Method for Generating Random Variables.
  *     Journal of Statistical Software, 5(8), 1-7 (2000).
  * [8] Saito, M. and Matsumoto, M.
  *     SIMD-Oriented Fast Mersenne Twister: a 128-bit Pseudorandom Number Generator
  *     Monte Carlo and Quasi-Monte Carlo Methods 2006, Springer, 607-622 (2008).
  *     doi:10.1007/978-3-540-85912-3_26
  * </pre>
  *
  * Example of generating 1D fractional Brownian motion:
  * \code
#include <stdio.h>
#include <libfbm.hpp>

int main()
{
  // we want one-dimensional (1D) fractional Brownian motion of size 1024
  libfbm::zvec dim(1);
  dim[0] = 1024;
  // create context for 1D fBm with exponent H=0.75
  libfbm::FWSContext ctx(0.75, dim);
  // set random generator seed, optional
  libfbm::gaussianRandomGenerator.setSeed(1);
  // set caching path, optional
  ctx.setCacheDir("/tmp/fbm");
  // create the field
  libfbm::Field fbm(ctx, true);
  // allocates memory and generates the fBm
  fbm.generate();
  // print it out
  for ( int i = 0; i < dim[0]; i++ )
      printf("%g\n", fbm(i));
  // generate another version of the fBm
  fbm.generate();
  // .. 
  return 0;
}
  * \endcode
  *
  * Example of generating 2D fractional Brownian surfaces:
  * \code
#include <stdio.h>
#include <libfbm.hpp>

int main()
{
  // create context for 2D fBs with exponent H=0.75 and size 64x64
  libfbm::FBMSteinContext ctx(0.75, 2, 64);
  // set random generator seed, optional
  libfbm::gaussianRandomGenerator.setSeed(1);
  // set caching path, optional
  ctx.setCacheDir("/tmp/fbm");
  // create the field
  libfbm::Field fbm(ctx, true);
  // allocates memory and generates the fBm
  fbm.generate();
  // print it out
  for ( int y = 0; y < fbm.getDim()[1]; y++ )
    {
      for ( int x = 0; x < fbm.getDim()[0]; x++ )
          printf("%s%g", x ? " " : "", fbm(x, y));
      printf("\n");
    }
  // generate another version of the fBm
  fbm.generate();
  // .. 
  return 0;
}
  * \endcode
  *
  * Example of generating 2D random process with custom covariance function:
  * \code
#include <stdio.h>
#include <math.h>
#include <libfbm.hpp>

// our own exponential unisotropic covariance function:
struct MyContext : public libfbm::SGPContext
{
  MyContext(const libfbm::zvec& dim) : libfbm::SGPContext(dim, dim, "myctx") { }

  double cov(const libfbm::zvec& r)
  {
    double xp = 0;
    for ( size_t i = 0; i < r.size(); i++ )
        xp += -fabs(r[i]) / (5 + i * 17);
    return exp(xp);
  }
};

int main()
{
  libfbm::zvec dim(2);
  dim[0] = 64;
  dim[1] = 64;
  MyContext ctx(dim);
  libfbm::Field fbm(ctx, true);
  fbm.generate();
  for ( int y = 0; y < dim[1]; y++ )
    {
      for ( int x = 0; x < dim[0]; x++ )
          printf("%s%g", x ? " " : "", fbm(x, y));
      printf("\n");
    }
  return 0;
}
  * \endcode
  *
  * And here's some legalese, the copyright notice for libfbm:
  * <pre>
  * Copyright (c) 2013, Indrek Mandre <indrek(at)mare.ee>
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met: 
  *
  * a Redistributions of source code must retain the above copyright notice, this
  *   list of conditions and the following disclaimer. 
  * b Redistributions in binary form must reproduce the above copyright notice,
  *   this list of conditions and the following disclaimer in the documentation
  *   and/or other materials provided with the distribution. 
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * </pre>
  *
  * Note that this software also uses FFTW, which is under the GPL. It should be trivial
  * to make changes to use a different FFT stack.
  *
  * Now despite the claims before, this software is not actually free. In case you used
  * it in research resulting in a published paper, or a thesis you defended,
  * you must send me a book. One book per paper. Any book will do. It can be used.
  * I do personally enjoy fiction.
  *
  * Take care and have fun!!
  *
  * Indrek Mandre - indrek(at)mare.ee - Institute of Cybernetics at the Tallinn University of Technology - Akadeemia tee 21, 12618, Tallinn, Estonia, EU
  *
  * This library incorporates SFMT, with the following license:
  * <pre>
  * Copyright (c) 2006,2007 Mutsuo Saito, Makoto Matsumoto and Hiroshima
  * University.
  * Copyright (c) 2012 Mutsuo Saito, Makoto Matsumoto, Hiroshima University
  * and The University of Tokyo.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  *     a Redistributions of source code must retain the above copyright
  *       notice, this list of conditions and the following disclaimer.
  *     b Redistributions in binary form must reproduce the above
  *       copyright notice, this list of conditions and the following
  *       disclaimer in the documentation and/or other materials provided
  *       with the distribution.
  *     c Neither the names of Hiroshima University, The University of
  *       Tokyo nor the names of its contributors may be used to endorse
  *       or promote products derived from this software without specific
  *       prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * </pre>
  * Note that the authors of SFMT have nothing to do with libfbm.
  *
  * TODO:
  *   * Maybe differentiate between dimension, dimensionality and size. Introduce rank?
  *     Quite a bit confusion in the API due to this right now. Bad indrek.
  *   * SIMD optimization for the Gaussian number generation.
  */

#ifndef __libfbm_hpp__
#define __libfbm_hpp__

#include <vector>
#include <string>

#include <stdint.h>

/** Maximum allowed dimension of the library.
  * 8 ought to be enough for everyone ;)
  * You can change this but you must then recompile the library.
  */
#define LIBFBM_MAX_DIM 8

#define LIBFBM_VERSION 0.3

namespace libfbm
{
  /** Normally distributed random number generator interface.
    * You can provide your own by extending this class and
    * passing it to Field::generate().
    */
  class GaussianGenerator
  {
  public:
    virtual ~GaussianGenerator();

    virtual void setSeed(uint32_t seed) = 0;
    virtual void setSeed(const unsigned char *seed_array, size_t len) = 0;

    virtual double getDouble() = 0;
    virtual void getDouble(double *array, size_t len) = 0;
  };

  #define _LIBFBM_GG_POOL_SIZE (312*4)

  /** SIMD-oriented Fast Mersenne Twister (SFMT) and Ziggurat sampling based gaussian random generator.
    * Very fast. Makes use of the excellent SFMT library by Mutsuo Saito and Makoto Matsumoto,
    * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/, also see [8].
    * It assumes that the CPU supports SSE2. Gaussian distribution is generated by a variation
    * of the ziggurat algorithm (basically layered rejection sampling). The ziggurat algorithm
    * used is slightly different from the one described by Marsaglia in [7], here we select
    * layers that cover an equal area of the distribution (instead of layers having similar
    * overall area).
    */
  class SFMTGaussianGenerator : public GaussianGenerator
  {
  public:
    SFMTGaussianGenerator(uint32_t seed = 0);
    SFMTGaussianGenerator(const unsigned char *seed_array, size_t len);
    ~SFMTGaussianGenerator();

    void setSeed(uint32_t seed);
    void setSeed(const unsigned char *seed_array, size_t len);

    double getDouble();
    void getDouble(double *array, size_t len);

  private:
    void *sfmt;
    uint64_t ipool[_LIBFBM_GG_POOL_SIZE];
    double opool[_LIBFBM_GG_POOL_SIZE];
    size_t ipindex;
    size_t opindex;

    void fill(double *array, size_t len);
  };

  /** Seed generator for random number generators. The Mersenne twister has a known weakness,
    * that is with a small integer seed the first generated values are not very random.
    * That is it takes a while for the generator to "work in".
    * To get over this one can use a larger seed. This class generates such seeds
    * from an integer index and a datafile containing random bits. On Unix one
    * can easily generate such a seed pool file by executing:
    * <pre>dd if=/dev/urandom of=seedpool.dat bs=1024 count=1024</pre>
    * The seeds generated are 256 bytes long and contain blocks from the
    * seed pool xor-ed with the given index. I'm not a specialist in random
    * number generation, so I hope this is adequate.
    */
  class Seeder
  {
  public:
    /** Construct the Seeder. The program aborts if the path cannot be opened or seed pool is too small (4kB).
      */
    Seeder(const char *path);
    ~Seeder();

    void seed(uint32_t index);

    inline size_t size() const { return sizeof(buf); }
    inline operator const unsigned char *() const { return (const unsigned char *)buf; }

  private:
    unsigned char buf[256];
    FILE *fp;

    Seeder(const Seeder&);
    const Seeder& operator=(const Seeder&);
  };

  /** Integer vector. The maximum dimension of the vector is hardcoded
    * in the LIBFBM_MAX_DIM (it is 8 by default). */
  class zvec
  {
  public:
    inline zvec(size_t dim) : clen(dim) { }

    inline int& operator[](size_t i) { return c[i]; }
    inline const int& operator[](size_t i) const { return c[i]; }

    inline size_t size() const { return clen; }

    /** Iterate the vector value using the given base.
      * @returns Returns false when values at all indices are equal to base. */
    inline bool increment(int base)
    {
      for ( size_t i = 0; i < size(); i++ )
        {
          if ( c[i] < base - 1 )
            {
              for ( size_t j = 0; j < i; j++ )
                  c[j] = 0;
              c[i]++;
              return true;
            }
        }
      return false;
    }

    /** Iterate the vector value using the given dimensions.
      * dim.size() must be the same as vector size.
      * @returns Returns false when values at all indices are equal to their dimension. */
    inline bool increment(const zvec& dim)
    {
      for ( size_t i = 0; i < size(); i++ )
        {
          if ( c[i] < dim[i] - 1 )
            {
              for ( size_t j = 0; j < i; j++ )
                  c[j] = 0;
              c[i]++;
              return true;
            }
        }
      return false;
    }

    /** Iterate the vector value using the given dimensions except for the dimension with index hold.
      * dim.size() must be the same as vector size.
      * @returns Returns false when values at all indices are equal to their dimension. */
    inline bool increment_but(const zvec& dim, size_t hold)
    {
      for ( size_t i = 0; i < size(); i++ )
        {
          if ( i == hold )
              continue;
          if ( c[i] < dim[i] - 1 )
            {
              for ( size_t j = 0; j < i; j++ )
                  c[j] = 0;
              c[i]++;
              return true;
            }
        }
      return false;
    }

    /** Serialized position of the vector. */
    inline size_t index(size_t base) const
    {
      size_t ret = 0;
      size_t mul = 1;
      for ( size_t i = 0; i < size(); i++ )
        {
          ret += c[i] * mul;
          mul *= base;
        }
      return ret;
    }

    /** Serialized position of the vector. */
    inline size_t index(const zvec& dim) const
    {
      size_t ret = 0;
      size_t mul = 1;
      for ( size_t i = 0; i < size(); i++ )
        {
          ret += c[i] * mul;
          mul *= dim[i];
        }
      return ret;
    }

    /** Zero all vector components. */
    inline void zero()
    {
      for ( size_t i = 0; i < size(); i++ )
          c[i] = 0;
    }

    friend inline zvec operator+(const zvec& l, const zvec& r)
    {
      zvec ret(l.size());
      for ( size_t i = 0; i < l.size(); i++ )
          ret[i] = l[i] + r[i];
      return ret;
    }

    friend inline zvec operator-(const zvec& l, const zvec& r)
    {
      zvec ret(l.size());
      for ( size_t i = 0; i < l.size(); i++ )
          ret[i] = l[i] - r[i];
      return ret;
    }

    friend inline zvec operator*(int f, const zvec& v)
    {
      zvec ret(v.size());
      for ( size_t i = 0; i < v.size(); i++ )
          ret[i] = f * v[i];
      return ret;
    }

    friend inline zvec operator*(const zvec& v, int f)
    {
      zvec ret(v.size());
      for ( size_t i = 0; i < v.size(); i++ )
          ret[i] = f * v[i];
      return ret;
    }

  private:
    int c[LIBFBM_MAX_DIM];
    size_t clen;
  };

  /** @cond INTERNAL
    * Cache.
    */
  class _Cache
  {
  public:
    _Cache(const std::string& cacheDir, const std::string& cacheName, size_t cacheSize);
    ~_Cache();

    bool load();
    void store(const std::vector<double>& data);

    size_t size() const { return cacheSize; }

    bool inMemory() const { return !data.empty(); }

  private:
    std::vector<double> data;
    FILE *fp;
    std::string cacheDir;
    std::string cacheName;
    size_t cacheSize;

    _Cache(const _Cache& copy);
    const _Cache& operator=(const _Cache& copy);

    friend class _CacheReader;
  };

  /** Cache reader.
    */
  class _CacheReader
  {
  public:
    _CacheReader(_Cache& cache, size_t bufferSize);

    inline double operator[](size_t i)
    {
      if ( !buf.empty() && (i < bufpos || i - bufpos >= buf.size()) )
          load(i);
      return data[i - bufpos];
    }

  private:
    _Cache& cache;
    std::vector<double> buf;
    size_t bufpos;
    double *data;

    void load(size_t i);
  };

  class FFT
  {
  public:
    enum edir_t {
      forward = 0,
      backward = 1
    };

    FFT();
    ~FFT();

    void init(const libfbm::zvec& dim, double *data, edir_t direction);

    void execute();

  private:
    void *plan;
  };
  /** @endcond */

  class Field;

  /** Abstract stationary Gaussian process context.
    * Used by the Field.
    * This class provides random number generation and filesystem based caching.
    * To define your own covariance function, you must create a class
    * that extends this.
    * Word of warning: if you have enabled caching, but change cov(), the results
    * from the old version are loaded. To overcome this you must manually delete
    * the old cache. To overcome this you should incorporate paramters affecting
    * cov() into the constructor's cacheName argument.
    */
  class SGPContext
  {
  public:
    /** Constructor.
      * The \a cacheName is the cache identifier used to distinct between cache files on the HD.
      * So if your extension of this class has some paramters affecting cov(), they should
      * be incorporated into cacheName. It is used in the file system path so must be
      * sane in that regard.
      */
    SGPContext(const zvec& fieldDim, const zvec& userDim, const std::string& cacheName);
    virtual ~SGPContext();

    virtual double cov(const zvec& p) = 0; /**< Covariance function. */

    /** Get the usable dimension of the generated field. */
    inline const zvec& getDim() const { return userDim; }
    /** Get the physical dimension of the generated field. It may be greater than the usable dimension returned by getDim(). */
    inline const zvec& getFieldDim() const { return fieldDim; }

    /** Set the cache directory. Must be called before construction of Field-s. */
    void setCacheDir(const std::string& cacheDir);

    /** Return the number of bad eigenvalues encountered. */
    inline size_t badEigenCount() const { return bec; }

    /** Initialize the cache. Either loads from cache (quick) or generates (slow).
      * This is called automatically by Field::generate().
      * @param forceRecalc Force recalculation and rewriting of the cache.
      * @returns Returns false in case negative eigenvalues were found. Otherwise returns true.
      */
    bool initCache(bool forceRecalc = false);

  protected:
    /** Postprocessor called after field generation.
      * This is called automatically by Field::generate().
      */
    virtual void postProcess(Field& field, GaussianGenerator& rng);

    /** Scale the output values by factor f. This is assembled into
      * the cached FFT matrix so must be called before initCache
      * or Field::generate(). If you change the value, you must
      * either erase the cache or incorporate it into cacheName.
      */
    inline void setScaleResult(double f) { scaleResult = f; }


  private:
    zvec fieldDim, userDim;
    _Cache *cache;
    std::string cacheDir;
    std::string cacheName;
    size_t bec;
    double scaleResult;

    friend class Field;

    SGPContext(const SGPContext&);
    const SGPContext& operator=(const SGPContext&);
  };

  /** Fractional gaussian noise context.
    * The covariance function is
    * \f[ \rho\!\left(\mathbf{r}\right)=\prod_{i=1}^{d}\frac{1}{2}\left[\left|r_i-1\right|^{2H}-2\left|r_i\right|^{2H}+\left|r_i+1\right|^{2H}\right], \f]
    * where \f$ 0<H<1 \f$. See [4]. For 1D cases the generated process is the increment process of fractional Brownian motion.
    */
  class FGNContext : public SGPContext
  {
  public:
    FGNContext(double H, const zvec& dim);

    double cov(const zvec& zvec);

  private:
    double H;
  };

  /** Fractional wiener surface context. 1D case it is equivalent to
    * fractional Brownian motion, for higher-dimensional cases the result is
    * a "fractional Wiener surface", see the paper by Qian [4].
    * It uses the same covariance function as fractional Gaussian noise, except
    * at post-processing the field is integrated (summed).
    */
  class FWSContext : public FGNContext
  {
  public:
    FWSContext(double H, const zvec& dim) : FGNContext(H, dim) { }

  protected:
    void postProcess(Field& field, GaussianGenerator& rng);
  };

  /** Base class for Field and PLFPSField. */
  class AbstractField
  {
  public:
    inline AbstractField() { }
    virtual ~AbstractField();

    /** Generate the field. */
    virtual void generate() = 0;
    /** Generate the field using random generator rng. The random generator
      * should produce normally distributed numbers.
      */
    virtual void generate(GaussianGenerator& rng) = 0;
    /** Release all memory this object holds. */
    virtual void clear() = 0;
    /** Get the usable dimensions of this field. */
    virtual const zvec& getDim() const  = 0;

    /** Get field value at the given point. Note that you are responsible
      * for correct point indices. No checking is done internally and
      * invalid values can lead to crashes. */
    inline double operator()(const zvec& p) const
    {
      size_t index = (size_t)2 * p[0];
      for ( size_t i = 1; i < p.size(); i++ )
          index += muls[i] * p[i];
      return datap[index];
    }
    inline double operator()(size_t x) const
    { return datap[2 * x]; }
    inline double operator()(size_t x, size_t y) const
    { return datap[2 * x + muls[1] * y]; }
    inline double operator()(size_t x, size_t y, size_t z) const
    { return datap[2 * x + muls[1] * y + muls[2] * z]; }
    inline double operator()(size_t x, size_t y, size_t z, size_t u) const
    { return datap[2 * x + muls[1] * y + muls[2] * z + muls[3] * u]; }
    inline double operator()(size_t x, size_t y, size_t z, size_t u, size_t v) const
    { return datap[2 * x + muls[1] * y + muls[2] * z + muls[3] * u + muls[4] * v]; }

    /** Return a reference to the internal array element at point \a p. */
    inline double& at(const zvec& p)
    {
      size_t index = 2 * p[0];
      for ( size_t i = 1; i < p.size(); i++ )
          index += muls[i] * p[i];
      return datap[index];
    }

    inline const double& at(const zvec& p) const
    {
      size_t index = 2 * p[0];
      for ( size_t i = 1; i < p.size(); i++ )
          index += muls[i] * p[i];
      return datap[index];
    }

    /** Get the strides array. Internally, the data is kept in a huge single
      * array of doubles. You can get addresses into this array using the at()
      * member functions. If you want to quickly move to the next or previous
      * element in a given dimension d, you can add or substract the strides[d]
      * value to the address. */
    inline const size_t *getStrides() const { return muls; }

  protected:
    std::vector<double> Z;
    double *datap;
    size_t muls[LIBFBM_MAX_DIM];

  private:
    AbstractField(const AbstractField&);
    const AbstractField& operator=(const AbstractField&);
  };

  /** Gaussian process field. This class holds the resulting data.
    * Note that when accessing the data you must be sure the indexes
    * are correct as no internal checking is done here (the same as
    * for a general C/C++ array/vector).
    */
  class Field : public AbstractField
  {
  public:
    /** Construct the field using given context.
      * Actual memory allocation is done at generate().
      * At each generate() two groups of random fields are generated,
      * each group containing 2^d cross-correlated processes. So at each
      * generate() we get exactly two uncorrelated random processes. One
      * of them is cached and switched in at the next call to generate().
      * However, if you want to also use the correlated fields,
      * set \a allowCorrelated to \a true.
      * So for 2D we get 4 instances, for 3D 8 instances, etc.
      * Note that the field holds onto the context object pointer,
      * so it must stick around until the field is destroyed.
      */
    Field(SGPContext& context, bool allowCorrelated = false);
    ~Field();

    /** Set the internal buffer size used when reading cached precomputed data
      * from the HD. This is 65536 by default. Don't worry, your OS is supposed
      * to cache often-used files in memory anyway, so probably there are no actual
      * file reads involved if you run your program properly. Just fast memor-to-memory
      * copies.
      */
    void setBufferSize(size_t bufferSize);

    /** Generate the next random process. At first calling this
      * function allocates the necessary memory. Note that multiple
      * instances of random processes are generated at one calculation,
      * which are quickly pulled in at next call to generate(). See the
      * constructor for more information.
      * The memory allocation size is 2*(2*d1)*(2*d2)*...*(2*dn) doubles,
      * where d1..dn are the dimensions of the generated process.
      * Note that the precomputed eigenvalue cache size (if kept in memory)
      * additionally adds half of that.
      */
    void generate();

    void generate(GaussianGenerator& rng);

    void clear();

    /** Access field value at given point. Note that you are responsible
      * for correct point indices. No checking is done internally and
      * invalid values can lead to crashes. */
    inline double operator()(const zvec& p) const
    {
      size_t index = (size_t)2 * p[0];
      for ( size_t i = 1; i < p.size(); i++ )
          index += muls[i] * p[i];
      return datap[index];
    }
    inline double operator()(size_t x) const
    { return datap[2 * x]; }
    inline double operator()(size_t x, size_t y) const
    { return datap[2 * x + muls[1] * y]; }
    inline double operator()(size_t x, size_t y, size_t z) const
    { return datap[2 * x + muls[1] * y + muls[2] * z]; }
    inline double operator()(size_t x, size_t y, size_t z, size_t u) const
    { return datap[2 * x + muls[1] * y + muls[2] * z + muls[3] * u]; }
    inline double operator()(size_t x, size_t y, size_t z, size_t u, size_t v) const
    { return datap[2 * x + muls[1] * y + muls[2] * z + muls[3] * u + muls[4] * v]; }

    /** Sum the generated field in each dimension.
      * Can be used to generate 1D fractional Brownian motion from the 1D fractional Gaussian noise.
      * For 2D case a strange fractional Wiener surface is produced (not very useful).
      * Note that FWSContext calls this automatically.
      */
    void integrate();

    /** Get the usable dimensions of this field. */
    inline const zvec& getDim() const { return context.getDim(); }

    /** Return a reference to the internal array element at point \a p. */
    inline double& at(const zvec& p)
    {
      size_t index = 2 * p[0];
      for ( size_t i = 1; i < p.size(); i++ )
          index += muls[i] * p[i];
      return datap[index];
    }

    inline const double& at(const zvec& p) const
    {
      size_t index = 2 * p[0];
      for ( size_t i = 1; i < p.size(); i++ )
          index += muls[i] * p[i];
      return datap[index];
    }

    /** Get the strides array. Internally, the data is kept in a huge single
      * array of doubles. You can get addresses into this array using the at()
      * member functions. If you want to quickly move to the next or previous
      * element in a given dimension d, you can add or substract the strides[d]
      * value to the address. */
    inline const size_t *getStrides() const { return muls; }

  private:
    FFT fft;
    SGPContext& context;
    bool allowCorrelated;
    size_t bufferSize;
    zvec psel;

    Field(const Field&);
    const Field& operator=(const Field&);
  };

  /** Class for testing the covariance of generated fields.
    * Note that in case of FBMSteinContext you should
    * disable post-processing. This basically generates
    * fields, directly calculates covariances and then compares
    * them to the cov() function provided by the context.
    */
  class SGPTester
  {
  public:
    /** Run an infinite test. Print status after every \a printInterval generation. */
    void test(SGPContext& context, size_t printInterval = 1);
  };

  /** Stein's fractional Brownian motion context.
    * Stein [3] defines fBm as a random process Z with a power-law increment variance:
    * \f[ \mbox{Var}\!\left(\left| Z\!\left(\mathbf{x}\right)-Z\!\left(\mathbf{y}\right)\right|^2\right)\propto\left|\mathbf{x}-\mathbf{y}\right|^{2H}, \f]
    * however, the covariance does not match that of a "standard" fBm.
    * Indeed, for Stein it is actually
    * \f[ \mbox{Cov}\!\left(Z\!\left(\mathbf{x}\right),Z\!\left(\mathbf{y}\right)\right)=c0-\left|\mathbf{x}-\mathbf{y}\right|^{2H}+c_2\left(\left|\mathbf{x}\right|^2+\left|\mathbf{y}^2\right|\right). \f]
    * The FBMSteinContext changes the actual genereated field dimension. However, only part of it, that is
    * the dimension given at construction is usable, the rest is used for the Stein's embedding method.
    * This means the getDim() and getFieldDim() may have different values. The Field::getDim() reflects
    * the usable area.
    */
  class FBMSteinContext : public SGPContext
  {
  public:
    /** Construct the Stein's FBM context object of size size^dim.
      * @param H The Hurst exponent.
      * @param dim Dimensionality of the field wanted (1D, 2D, 3D, etc.).
      * @param size Request field of size^dim.
      * @param Rhint Hint that overrides the self-determined R (see getRforH()).
      *   See [3] for details but in general 1<=R<=2. Larger R wastes more space and takes more time
      *   to calculate; smaller R may result in negative eigenvalues and a failed calculation.
      *   So one strives towards smallest R possible that works (no negative eigenvalues).
      *   As field size is increased (or field dimension), R must also increase.
      *   If left negative, the program attempts to estimate the value based on parameter H and
      *   dimensionality, but this estimate is conservative.
      * @param mapDim Expand dimension so that the resulting usable field is the size requested. True by default.
      *
      * The automatic R detections seems to work well for 2D case.
      * For higher-dimensional cases you may have to fiddle with it manually.
      */
    FBMSteinContext(double H, size_t dim, size_t size, double Rhint = -1, bool mapDim = true);

    double cov(const zvec& zvec);

  protected:
    void postProcess(Field& field, GaussianGenerator& rng);

  public:
    void disablePostProcessing();

    /** Get the parameter \a R described in [3]. */
    double getR() const { return R; }

    /** Get the required field size for the given usable user \a size and \a R.
      * Stein's method [2] requires a larger field to generate a field
      * for the required initial dimensions. This size depends on the chosen R.
      * For 2D and H <= 0.75, R is always 1.0. For H > 0.75, R must be increased. It is
      * proven that the methdod works with R=2.0. However, in practice R=1.3 at H=0.999 works
      * just fine.
      */
    static size_t userSize2FieldSize(size_t size, double R);
    /** Map the field dimension into the resulting usable fBm field dimension. */
    static size_t fieldSize2UserSize(size_t size, double R);
    /** Get the precomputed R value for the given H. This function
      * is quantized, so is not optimal. Also R depends a bit on
      * the size of the field. This here is conservative. However,
      * it could be it fails for some cases, then you need to provide
      * your own correct R. The returned value is a very conservative
      * 2.0 when dim >= 3, so for 3D and higher-dimensional cases
      * it makes sense to find your own optimal R value.
      * @param Rhint If equal or greater than 1, return Rhint.
      */
    static double getRForH(double H, size_t dim, double Rhint = -1);

  private:
    double H;
    double R;
    double beta, c0, c2;
    double Rscale;
    bool pp;
  };

  /** Power law covariance context.
    * \f[ \rho\!\left(\mathbf{r}\right)=\left|\mathbf{r}\right|^{2H}, \f]
    * where \f$H<0\f$.
    */
  class PowerLawContext : public SGPContext
  {
  public:
    /** Constructor. \a Var determines the variance or cov(0).
      * This parameter strongly affects whether circular matrix embedding works or not.
      * You can decrease this to 1 when \f$H << 0\f$.
      * Note that as H approaches 0 the field becomes distorted.
      * H=0.01 should still be quite fine.
      */
    PowerLawContext(double H, const zvec& dim, double Var = 2.0);

    double cov(const zvec& dim);

  private:
    double H, Var;
  };

  /** Power-law power spectrum field. Loosely based on [6].
    * The power spectum is given as \f[ S\!\left(f\right)\sim 1/f^{(2H+d)}. \f]
    * We fill a complex matrix with gaussian random values. We don't use
    * symmetric conjugation. The random values are scaled based on frequency,
    * that is they are scaled using the power law above as scaling factor.
    * The values are also globally scaled so that the resulting field has
    * variance 1.0. Then a FFT is applied. This result in two fields, one
    * in the real part, the other in the complex part. The real part is
    * exported first, the next generate() switches to the imaginary part.
    * Note that the fields in this case are symmetric. The 0 frequency
    * component is taken as 0. The scaling factors are kept in memory,
    * so total memory usage is 3x that of field dimension.
    */
  class PLFPSField : public AbstractField
  {
  public:
    PLFPSField(const zvec& dim, double H);
    ~PLFPSField();

    void generate();
    void generate(GaussianGenerator& rng);

    void clear();

    const zvec& getDim() const { return dim; }

  private:
    zvec dim;
    FFT fft;
    double H;
    std::vector<double> cache;
    size_t len;

    bool initCache();
  };

  /** Global random generator used by default in Field::generate(). */
  extern SFMTGaussianGenerator gaussianRandomGenerator;
};

#endif // !__libfbm_hpp__

