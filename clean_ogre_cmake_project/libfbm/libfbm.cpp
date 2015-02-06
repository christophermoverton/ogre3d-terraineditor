/** libfbm, http://www.mare.ee/indrek/libfbm/
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
  */

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <sstream>

#include <fftw3.h>

#include "libfbm.hpp"

#define SFMT_MEXP 19937
#include "SFMT.h"

static std::string zvec2str(const libfbm::zvec& v)
{
  std::stringstream ss;
  for ( size_t i = 0; i < v.size(); i++ )
    {
      if ( i )
          ss << "x";
      ss << v[i];
    }
  return ss.str();
}

// not efficient, but this is only called couple of times
static std::string dbl2str(double dbl)
{
  char buf[256];
  sprintf(buf, "%g", dbl);
  return buf;
}

#if 0
// we've switched to fftw now. GSL fft is dog slow.
// N-dimensional fast fourier transform
static void FFTND(std::vector<double>& D, const libfbm::zvec& dim, bool forward)
{
  gsl_fft_complex_wavetable *wavetable = 0;
  gsl_fft_complex_workspace *workspace = 0;
  libfbm::zvec itp(dim.size()); // using this to cycle through the N-1 indices
  size_t r = 1; // separation distance of data lements along a column in the data vector D
  std::vector<double> buf; // buffer for the column, stride in gsl fft is 10x slower

  // we run 1D FFT through all columns (with fixed N-1 dimensions)
  // so d is the dimension the column spans
  for ( size_t d = 0; d < dim.size(); d++ )
    {
      bool pt = isPowerOfTwo(dim[d]);

      if ( !pt )
        {
          wavetable = gsl_fft_complex_wavetable_alloc(dim[d]);
          workspace = gsl_fft_complex_workspace_alloc(dim[d]);
        }

      itp.zero();

      while(1)
        {
          // find first element address inside data
          double *dp = &D[2 * itp.index(dim)];
          double *wp = dp;

          if ( d > 0 ) // need to fetch the data into a buffer
            {
              buf.clear();
              buf.reserve(2 * dim[d]);
              for ( int i = 0; i < dim[d]; i++ )
                {
                  buf.push_back(*(dp + 2 * i * r));
                  buf.push_back(*(dp + 2 * i * r + 1));
                }
              wp = &buf[0];
            }

#if 0
          printf("fft(");
          for ( int i = 0; i < dim[d]; i++ )
            {
              if ( i ) printf(", ");
              printf("%.3g", wp[i * 2]);
            }
          printf(")\n");
#endif

          if ( pt )
              gsl_fft_complex_radix2_transform(wp, 1, dim[d], forward ? gsl_fft_forward : gsl_fft_backward);
          else
              gsl_fft_complex_transform(wp, 1, dim[d], wavetable, workspace, forward ? gsl_fft_forward : gsl_fft_backward);

          if ( d > 0 )
            {
              buf.clear();
              buf.reserve(2 * dim[d]);
              for ( int i = 0; i < dim[d]; i++ )
                {
                  *(dp + 2 * i * r) = buf[2 * i];
                  *(dp + 2 * i * r + 1) = buf[2 * i + 1];
                }
            }

          // cycle through all combinations in other dimensions
          if ( !itp.increment_but(dim, d) )
              break;
        }

      if ( !pt )
        {
          gsl_fft_complex_workspace_free(workspace);
          gsl_fft_complex_wavetable_free(wavetable);
        }

      r *= dim[d];
    }
}
#endif

libfbm::FFT::FFT() : plan(0)
{
}

libfbm::FFT::~FFT()
{
  if ( plan )
      fftw_destroy_plan((fftw_plan)plan);
}

void libfbm::FFT::execute()
{
  fftw_execute((fftw_plan)plan);
}

void libfbm::FFT::init(const libfbm::zvec& dim, double *data, edir_t direction)
{
  if ( plan )
      fftw_destroy_plan((fftw_plan)plan);
  size_t len = 1;
  int n[LIBFBM_MAX_DIM];
  for ( size_t i = 0; i < dim.size(); i++ )
    {
      n[dim.size() - 1 - i] = dim[i];
      len *= dim[i];
    }
  plan = fftw_plan_dft(dim.size(), n, (fftw_complex*)data, (fftw_complex*)data, direction == forward ? FFTW_FORWARD : FFTW_BACKWARD, FFTW_MEASURE);
  assert(plan);
}

libfbm::GaussianGenerator::~GaussianGenerator()
{
}

#define SFMT ((sfmt_t*)sfmt)

libfbm::SFMTGaussianGenerator::SFMTGaussianGenerator(uint32_t seed)
{
  sfmt = new sfmt_t();
  setSeed(seed);
}

libfbm::SFMTGaussianGenerator::SFMTGaussianGenerator(const unsigned char *seed_array, size_t len)
{
  sfmt = new sfmt_t();
  setSeed(seed_array, len);
}

libfbm::SFMTGaussianGenerator::~SFMTGaussianGenerator()
{
  delete SFMT;
}

void libfbm::SFMTGaussianGenerator::setSeed(uint32_t seed)
{
#if 0
  fprintf(stderr, "setSeed: %x\n", seed);
#endif
  sfmt_init_gen_rand(SFMT, seed);
  ipindex = opindex = _LIBFBM_GG_POOL_SIZE;
}

void libfbm::SFMTGaussianGenerator::setSeed(const unsigned char *seed_array, size_t len)
{
  size_t n = len / sizeof(uint32_t);
#if 0
  fprintf(stderr, "setSeed: ");
  for ( size_t i = 0; i < n; i++ )
      fprintf(stderr, "%08x", ((uint32_t*)seed_array)[i]);
  fprintf(stderr, "\n");
#endif
  sfmt_init_by_array(SFMT, (uint32_t*)seed_array, n);
  ipindex = opindex = _LIBFBM_GG_POOL_SIZE;
}

#if 0
// perhaps in future

//typedef uint64_t vui64_t __attribute__ ((vector_size (16)));
//typedef double vd_t __attribute__ ((vector_size (16)));

#include <emmintrin.h>

struct iv_t
{
  inline uint64_t operator[](size_t index) const { return u.i[index]; }
  inline uint64_t& operator[](size_t index) { return u.i[index]; }

  union
  {
    __m128i v;
    uint64_t i[2];
  } u;

  friend inline iv_t operator+(const iv_t& l, const iv_t& r)
  {
    iv_t ret;
    ret.u.v = _mm_add_epi64(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator-(const iv_t& l, const iv_t& r)
  {
    iv_t ret;
    ret.u.v = _mm_sub_epi64(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator<<(const iv_t& l, const iv_t& r)
  {
    iv_t ret;
    ret.u.v = _mm_sll_epi64(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator<<(const iv_t& l, int count)
  {
    iv_t ret;
    ret.u.v = _mm_slli_epi64(l.u.v, count);
    return ret;
  }

  friend inline iv_t operator>>(const iv_t& l, const iv_t& r)
  {
    iv_t ret;
    ret.u.v = _mm_srl_epi64(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator>>(const iv_t& l, int count)
  {
    iv_t ret;
    ret.u.v = _mm_srli_epi64(l.u.v, count);
    return ret;
  }

  friend inline iv_t operator&(const iv_t& l, const iv_t& r)
  {
    iv_t ret;
    ret.u.v = _mm_and_si128(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator|(const iv_t& l, const iv_t& r)
  {
    iv_t ret;
    ret.u.v = _mm_or_si128(l.u.v, r.u.v);
    return ret;
  }

  static inline iv_t make(uint64_t a, uint64_t b)
  {
    iv_t ret;
    ret.u.i[0] = a;
    ret.u.i[1] = b;
    return ret;
  }
};

struct dv_t
{
  inline double operator[](size_t index) const { return u.d[index]; }
  inline double& operator[](size_t index) { return u.d[index]; }

  union
  {
    __m128d v;
    double d[2];
  } u;

  friend inline dv_t operator+(const dv_t& l, const dv_t& r)
  {
    dv_t ret;
    ret.u.v = _mm_add_pd(l.u.v, r.u.v);
    return ret;
  }

  friend inline dv_t operator-(const dv_t& l, const dv_t& r)
  {
    dv_t ret;
    ret.u.v = _mm_sub_pd(l.u.v, r.u.v);
    return ret;
  }

  friend inline dv_t operator*(const dv_t& l, const dv_t& r)
  {
    dv_t ret;
    ret.u.v = _mm_mul_pd(l.u.v, r.u.v);
    return ret;
  }

  friend inline dv_t operator/(const dv_t& l, const dv_t& r)
  {
    dv_t ret;
    ret.u.v = _mm_div_pd(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator<=(const dv_t& l, const dv_t& r)
  {
    iv_t ret;
    ret.u.v = (__m128i)_mm_cmple_pd(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator<(const dv_t& l, const dv_t& r)
  {
    iv_t ret;
    ret.u.v = (__m128i)_mm_cmplt_pd(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator>=(const dv_t& l, const dv_t& r)
  {
    iv_t ret;
    ret.u.v = (__m128i)_mm_cmpge_pd(l.u.v, r.u.v);
    return ret;
  }

  friend inline iv_t operator>(const dv_t& l, const dv_t& r)
  {
    iv_t ret;
    ret.u.v = (__m128i)_mm_cmpgt_pd(l.u.v, r.u.v);
    return ret;
  }

  static inline dv_t make(double a, double b)
  {
    dv_t ret;
    ret.u.d[0] = a;
    ret.u.d[1] = b;
    return ret;
  }
};

union v_t
{
  iv_t i;
  dv_t d;
};
#endif

#define LOW_MASK    0x000FFFFFFFFFFFFFULL
#define HIGH_MASK   0xFFF0000000000000ULL
#define HIGH_SHIFT (13*4)
#define HIGH_CONST  0x3FF0000000000000ULL
#define SIGN_MASK   0x8000000000000000ULL

#include "zig.h"

// we have 12 bits of goodness!!

union tu_t
{
  uint64_t i;
  double d;
};

static inline double signdbl(double d, uint64_t b)
{
  tu_t v;
  v.d = d;
  v.i |= b;
  return v.d;
}

static inline double ndistf(double x)
{
  return exp(-x * x / 2);
}

double libfbm::SFMTGaussianGenerator::getDouble()
{
  if ( opindex == _LIBFBM_GG_POOL_SIZE )
    {
      fill(opool, _LIBFBM_GG_POOL_SIZE);
      opindex = 0;
    }
  return opool[opindex++];
}

static inline uint64_t ipnext(sfmt_t *sfmt, uint64_t *pool, size_t& ipindex)
{
  if ( ipindex == _LIBFBM_GG_POOL_SIZE )
    {
      sfmt_fill_array64(SFMT, (uint64_t*)pool, _LIBFBM_GG_POOL_SIZE); 
      ipindex = 0;
    }
  return pool[ipindex++];
}

void libfbm::SFMTGaussianGenerator::fill(double *array, size_t len)
{
  for ( size_t i = 0; i < len; i++ )
    {
      tu_t v;
      v.i = ipnext(SFMT, ipool, ipindex);
      uint64_t sbit = v.i & SIGN_MASK;
      size_t layer = ((v.i >> HIGH_SHIFT) & ZIG_MASK);
      v.i = (v.i & LOW_MASK) | HIGH_CONST;

      double x = (v.d - 1) * slices[layer + 1].x;
      // this should pass 99.8% (for 2048 layers)
      if ( (v.i & LOW_MASK) < slices[layer + 1].cutx )
        {
          array[i] = signdbl(x, sbit);
          continue;
        }

      if ( layer != ZIG_SLICES - 1 )
        {
          while ( 1 )
            {
              // test with y
              //if ( x < slices[layer].x )
              v.i = ipnext(SFMT, ipool, ipindex);
              v.i = (v.i & LOW_MASK) | HIGH_CONST;
              double y = (v.d - 1) * (slices[layer].y - slices[layer + 1].y);
              if ( y < ndistf(x) - slices[layer + 1].y )
                {
                  array[i] = signdbl(x, sbit);
                  break;
                }

              // re-generate x and try again
              v.i = ipnext(SFMT, ipool, ipindex);
              v.i = (v.i & LOW_MASK) | HIGH_CONST;
              x = (v.d - 1) * slices[layer + 1].x;
              if ( (v.i & LOW_MASK) < slices[layer + 1].cutx )
                {
                  array[i] = signdbl(x, sbit);
                  break;
                }
            }
        }
      else
        {
          //double x = (v.d - 1) * ZIG_TAIL_FACTOR;
          double r = slices[ZIG_SLICES - 1].x;
          while(1)
            {
              v.i = ipnext(SFMT, ipool, ipindex);
              v.i = (v.i & LOW_MASK) | HIGH_CONST;
              double x = -log(v.d - 1.0) / r;
              v.i = ipnext(SFMT, ipool, ipindex);
              v.i = (v.i & LOW_MASK) | HIGH_CONST;
              double y = -log(v.d - 1.0);
              // If x is -Inf, the condition never passes, so this is ok, as y is not used in the result.
              if ( 2 * y > x * x )
                {
                  array[i] = signdbl(x + r, sbit);
                  break;
                }
            }
        }
    }
}

void libfbm::SFMTGaussianGenerator::getDouble(double *array, size_t len)
{
  size_t i = 0;
  while ( opindex != _LIBFBM_GG_POOL_SIZE && i != len )
      array[i++] = opool[opindex++];

  if ( i != len )
      fill(&array[i], len - i);
}

libfbm::_Cache::_Cache(const std::string& cacheDir, const std::string& cacheName, size_t cacheSize)
  : fp(0), cacheDir(cacheDir), cacheName(cacheName), cacheSize(cacheSize)
{
}

bool libfbm::_Cache::load()
{
  if ( cacheDir.empty() )
      return false;

  char path[1024];
  sprintf(path, "%s/%s.%zu.dat", cacheDir.c_str(), cacheName.c_str(), cacheSize);

  struct stat st;
  mkdir (cacheDir.c_str(), 0700);
  assert(lstat(cacheDir.c_str(), &st) == 0 && "unable to create cache directory");
  assert(st.st_uid == geteuid() && "cache directory not owned by us"); // security check against malicious attacker

  fp = fopen(path, "rb");
  if ( fp )
    {
      fseeko(fp, 0, SEEK_END);
      if ( ftello(fp) == (off_t)(cacheSize * sizeof(double)) )
          return true;
      fclose(fp);
      fp = 0;
    }

  return false;
}

void libfbm::_Cache::store(const std::vector<double>& data)
{
  assert(cacheSize == data.size());

  if ( cacheDir.empty() )
    {
      this->data = data;
      return;
    }

  char path[1024];
  sprintf(path, "%s/%s.%zu.dat", cacheDir.c_str(), cacheName.c_str(), cacheSize);

  struct stat st;
  mkdir (cacheDir.c_str(), 0700);
  assert(lstat(cacheDir.c_str(), &st) == 0 && "unable to create cache directory");
  assert(st.st_uid == geteuid() && "cache directory not owned by us"); // security check against malicious attacker

  fp = fopen(path, "wb");
  assert(fp && "unable to open the cache file for writing");
  fwrite(&data[0], data.size() * sizeof(double), 1, fp);
  fclose(fp);

  fp = fopen(path, "rb");
  assert(fp && "unable to open the cache file for reading");
}

libfbm::_Cache::~_Cache()
{
  if ( fp )
      fclose(fp);
}

libfbm::_CacheReader::_CacheReader(_Cache& cache, size_t bufferSize) : cache(cache)
{
  if ( cache.inMemory() ) // memory-based cache
    {
      bufpos = 0;
      data = &cache.data[0];
    }
  else // file-based cache
    {
      if ( bufferSize > cache.cacheSize )
          bufferSize = cache.cacheSize;
      bufpos = (size_t)-1;
      buf.resize(bufferSize);
      data = &buf[0];
    }
}

void libfbm::_CacheReader::load(size_t i)
{
  bufpos = i - i % buf.size();
  //fprintf(stderr, "%p: load: [%zu, %zu)\n", this, bufpos, buf.size());
  assert(!buf.empty());
  assert(fseeko(cache.fp, bufpos * sizeof(double), SEEK_SET) == 0);
  fread(&buf[0], sizeof(double) * buf.size(), 1, cache.fp);
}

libfbm::SGPContext::SGPContext(const zvec& fieldDim, const zvec& userDim, const std::string& cacheName)
  : fieldDim(fieldDim), userDim(userDim), cache(0), cacheName(cacheName + "." + zvec2str(fieldDim))
{
  assert(fieldDim.size() <= LIBFBM_MAX_DIM);
  bec = 0;
  scaleResult = 1;
}

libfbm::SGPContext::~SGPContext()
{
  delete cache;
}

void libfbm::SGPContext::postProcess(Field& field, GaussianGenerator& rng)
{
}

void libfbm::SGPContext::setCacheDir(const std::string& cacheDir)
{
  this->cacheDir = cacheDir;
}

bool libfbm::SGPContext::initCache(bool forceRecalc)
{
  if ( cache && forceRecalc )
    {
      delete cache;
      cache = 0;
    }

  if ( cache )
      return true;

  size_t len = 1;
  zvec ddim(fieldDim.size());
  for ( size_t i = 0; i < fieldDim.size(); i++ )
    {
      assert(fieldDim[i] >= 2);
      ddim[i] = fieldDim[i] * 2;
      len *= 2 * fieldDim[i];
    }

  cache = new _Cache(cacheDir, cacheName, len);
  if ( !forceRecalc && cache->load() )
      return true;

  std::vector<double> IL;
  IL.resize(2 * len, 0);

  FFT fft;
  fft.init(ddim, &IL[0], FFT::backward);

  std::vector<zvec> pcov;
  double *wp = &IL[0];

  zvec p(fieldDim.size());
  p.zero();
  while(1)
    {
#if 0
      for ( size_t i = 0; i < p.size(); i++ )
          printf(" %d", p[i]);
      printf("\n");
#endif
      pcov.clear();
      pcov.push_back(p);
      for ( size_t i = 0; i < p.size(); i++ )
        {
          int newval1, newval2;

          if ( p[i] < fieldDim[i] )
            {
              newval1 = newval2 = p[i];
            }
          else if ( p[i] > fieldDim[i] )
            {
              newval1 = newval2 = p[i] - 2 * fieldDim[i];
            }
          else
            {
              newval1 = fieldDim[i] - 1;
              newval2 = fieldDim[i] + 1 - 2 * fieldDim[i];
            }

          size_t clen = pcov.size();
          for ( size_t j = 0; j < clen; j++ )
            {
              pcov[j][i] = newval1;
              if ( newval1 != newval2 )
                {
                  pcov.push_back(pcov[j]);
                  pcov.back()[i] = newval2;
                }
            }
        }
      double val = 0;
      for ( size_t i = 0; i < pcov.size(); i++ )
          val += cov(pcov[i]);
      *wp++ = val / pcov.size();
      wp++;
      if ( !p.increment(ddim) )
          break;
    }

#if 0
  for ( size_t i = 0; i < IL.size() / 2; i++ )
    {
      printf ("   %1.5f", IL[2 * i]);
      if ( i && !((i + 1) % 8) )
          printf("\n");
    }
#endif

  fft.execute();

  bec = 0;
  double ttrace = 0;
  double ptrace = 0;
  for ( size_t i = 0; i < IL.size(); i += 2 )
    {
      ttrace += IL[i];
      if ( IL[i] < 0 )
        { 
          if ( IL[i] < -1e-14 ) // allow some numeric errors
            {
              bec++;
              //printf("Neg: %g\n", IL[i]);
            }
          IL[i] = 0;
        }
      ptrace += IL[i];
      IL[i/2] = sqrt(IL[i] / len) * scaleResult;
    }
  IL.resize(IL.size() / 2);
  if ( bec )
    {
      fprintf(stderr, "Zeroed %zu/%zu of negative eigenvalues (matrix not nonnegative definite, generated noise will be approximate)\n", bec, IL.size());
      double mul = sqrt(ttrace / ptrace);
      for ( size_t i = 0; i < IL.size(); i++ )
          IL[i] *= mul;
    }

#if 0
  printf("Results:\n");
  for ( size_t i = 0; i < IL.size(); i++ )
    {
      printf ("   %1.5f", IL[i]);
      if ( i && !((i + 1) % 8) )
          printf("\n");
    }
#endif

  cache->store(IL);

  return bec == 0;
}

libfbm::AbstractField::~AbstractField()
{
}

libfbm::Field::Field(SGPContext& context, bool allowCorrelated)
  : context(context), allowCorrelated(allowCorrelated), bufferSize(65536),
    psel(!allowCorrelated ? 1 : (context.getFieldDim().size() + 1))
{
  memset(muls, 0, sizeof(muls));
  size_t r = 1;
  for ( size_t i = 0; i < context.getFieldDim().size(); i++ )
    {
      muls[i] = 2 * r;
      r = r * 2 * context.getFieldDim()[i];
    }
}

libfbm::Field::~Field()
{
}

void libfbm::Field::setBufferSize(size_t bufferSize)
{
  this->bufferSize = bufferSize;
}

void libfbm::Field::clear()
{
  std::vector<double> cl;
  Z.swap(cl);
}

void libfbm::Field::generate()
{
  generate(gaussianRandomGenerator);
}

void libfbm::Field::generate(GaussianGenerator& rng)
{
  zvec ddim = context.getFieldDim() * 2;

  if ( !Z.empty() && psel.increment(2) )
    {
      size_t offset = 0;
      if ( psel[0] )
          offset++;
      if ( allowCorrelated )
        {
          size_t r = 1;
          for ( size_t i = 1; i < psel.size(); i++ )
            {
              if ( psel[i] )
                  offset += 2 * r * context.getFieldDim()[i - 1];
              r *= ddim[i - 1];
            }
        }
      datap = &Z[offset];
      context.postProcess(*this, rng);
      return;
    }

  if ( !context.cache )
      context.initCache();
  _CacheReader LF(*context.cache, bufferSize);

  if ( Z.empty() )
    {
      Z.resize(2 * context.cache->size());
      fft.init(ddim, &Z[0], FFT::forward);
    }
  datap = &Z[0];
  psel.zero();

  rng.getDouble(&Z[0], Z.size());

  for ( size_t i = 0; i < Z.size() / 2; i++ )
    {
      Z[i * 2] = LF[i] * Z[i * 2];
      Z[i * 2 + 1] = LF[i] * Z[i * 2 + 1];
    }

  fft.execute();
  context.postProcess(*this, rng);
}

void libfbm::Field::integrate()
{
  // we must basically cumulatively sum all columns in all directions
  // so just like in the FFTND, but instead of fft we sum
  zvec dim = context.getFieldDim();

  libfbm::zvec itp(dim.size()); // using this to cycle through the N-1 indices

  for ( size_t d = 0; d < dim.size(); d++ )
    {
      itp.zero();
      size_t r = muls[d];

      while(1)
        {
          // find first element address inside data
          double *dp = &at(itp);

          double sum = 0;
          for ( int i = 0; i < dim[d]; i++ )
            {
              sum += dp[i * r];
              dp[i * r] = sum;
            }

          // cycle through all combinations in other dimensions
          if ( !itp.increment_but(dim, d) )
              break;
        }
    }
}

libfbm::FGNContext::FGNContext(double H, const zvec& dim)
  : SGPContext(dim, dim, "fgn." + dbl2str(H)), H(H)
{
}

double libfbm::FGNContext::cov(const zvec& p)
{
  double ret = 1;
  for ( size_t i = 0; i < p.size(); i++ )
    {
      double r = fabs(p[i]);
      ret *= 0.5 * (pow((r - 1) * (r - 1), H) - 2 * pow(r * r, H) + pow((r + 1) * (r + 1), H));
    }
  return ret;
}

void libfbm::FWSContext::postProcess(Field& field, GaussianGenerator& rng)
{
  field.integrate();
}

static libfbm::zvec dim2dim(size_t size, size_t dim)
{
  assert(dim <= LIBFBM_MAX_DIM);
  libfbm::zvec ret(dim);
  for ( size_t i = 0; i < dim; i++ )
      ret[i] = size;
  return ret;
}

#define CONSERVATIVE_R 1.3

//  {4096, {{0.775, 1.00079}, {0.8, 1.00108}, {0.825, 1.0084}, {0.85, 1.02536}, {0.875, 1.05198}, {0.9, 1.08649}, {0.925, 1.12773}, {0.95, 1.17189}, {0.975, 1.21928}, {0.99, 1.26256}}},
static double R_map2[][2] = {
  {0.775, 1.001},
  {0.8,   1.002},
  {0.825, 1.009},
  {0.85,  1.026},
  {0.875, 1.052},
  {0.9,   1.087},
  {0.925, 1.128},
  {0.95,  1.172},
  {0.975, 1.220},
  {0.99,  1.263},
  {1,     CONSERVATIVE_R},
  {0,     0}
};

size_t libfbm::FBMSteinContext::userSize2FieldSize(size_t size, double R)
{
  size = ceil(R * size * sqrt(2) + 1e-16);
  while ( size % 8 )
      size++; // get some even-ness to optimize the FFT
  return size;
}

double libfbm::FBMSteinContext::getRForH(double H, size_t dim, double R)
{
  if ( R >= 1 )
      return R;

  if ( dim > 2 )
      return 2.0;

  if ( H <= 0.75 )
      return 1.0;

  if ( R < 1 )
    {
      size_t i = 0;
      while ( R_map2[i][0] )
        {
          if ( H <= R_map2[i][0] )
            {
              R = R_map2[i][1];
              //printf("Found: %g\n", R);
              break;
            }
          i++;
        }
      if ( R < 1 )
          R = CONSERVATIVE_R;
    }

  return R;
}

size_t libfbm::FBMSteinContext::fieldSize2UserSize(size_t size, double R)
{
  return floor(size / (R * sqrt(2)) - 1e-16);
}

// This is kind of insane. Having the dimension in the constructor actually
// makes this really cumbersome and ugly. It's convenient for the user. Oh well.
libfbm::FBMSteinContext::FBMSteinContext(double H, size_t dim, size_t size, double Rhint, bool mapDim)
  : SGPContext(
      dim2dim(mapDim ? userSize2FieldSize(size, getRForH(H, dim, Rhint)) : size, dim),
      dim2dim(mapDim ? size : fieldSize2UserSize(size, getRForH(H, dim, Rhint)), dim),
      "stein." + dbl2str(H) + "." + dbl2str(getRForH(H, dim, Rhint))),
    H(H), R(getRForH(H, dim, Rhint)), pp(true)
{
  if ( R == 1.0 )
    {
      beta = 0;
      c2 = H;
      c0 = 1 - c2;
    }
  else
    {
      beta = 2 * H * (2 - 2 * H) / (3 * R * (R * R - 1));
      c2 = (2 * H - beta * (R - 1) * (R - 1) * (R + 2)) / 2;
      c0 = beta * (R - 1) * (R - 1) * (R - 1) + 1 - c2;
    }
  Rscale = R / (getFieldDim()[0] - 1) + 1e-16;
  //assert(floor((sqrt(2) / 2) * (1 / Rscale) - 1e-16) >= dim[0] - 1);

  setScaleResult(0.5 * sqrt(2) * pow(Rscale, -H));
}

double libfbm::FBMSteinContext::cov(const zvec& p)
{
  uint64_t len2 = 0;
  for ( size_t i = 0; i < p.size(); i++ )
      len2 += (uint64_t)p[i] * p[i];
  double len = sqrt(len2) * Rscale;
  double ret;
  if ( len <= 1 )
    {
      ret = c0 + c2 * len * len - pow(len, 2 * H);
    }
  else if ( len <= R )
    {
      ret = beta * (R - len) * (R - len) * (R - len) / len;
    }
  else
    {
      ret = 0;
    }
  //printf("cov(%d,%d) len=%g ret=%g\n", p[0], p[1], len, ret);
  return ret;
}

void libfbm::FBMSteinContext::postProcess(Field& field, GaussianGenerator& rng)
{
  if ( !pp )
      return;
  double scalingCorrector = 0.5 * sqrt(2) * pow(Rscale, -H);

  int range = getDim()[0];
  const zvec& dim = getFieldDim();

  std::vector<double> Z;
  Z.reserve(dim.size());
  for ( size_t i = 0; i < dim.size(); i++ )
      Z.push_back(sqrt(2 * c2) * rng.getDouble());

  std::vector<double> r;
  r.reserve(dim.size() * range);
  for ( size_t d = 0; d < dim.size(); d++ )
      for ( int i = 0; i < range; i++ )
          r.push_back(i * Rscale * Z[d] * scalingCorrector);

  zvec p(dim.size());
  p.zero();
  double corner = field(p);

  while (1)
    {
      double sum = -corner;
      for ( size_t i = 0; i < p.size(); i++ )
          sum += r[i * range + p[i]];
      double& v = field.at(p);
      v = (v + sum);
      if ( !p.increment(range) )
          break;
    }
}

void libfbm::FBMSteinContext::disablePostProcessing()
{
  pp = false;
}

void libfbm::SGPTester::test(SGPContext& context, size_t printInterval)
{
  Field fbm(context, true);

  const zvec& dim = context.getFieldDim();
  size_t len = 1;
  for ( size_t i = 0; i < dim.size(); i++ )
      len *= dim[i];
  size_t totE = 2 * ((size_t)1 << dim.size());

  std::vector<long double> sums(len * len * totE, 0);

  size_t count = 0;
  while(1)
    {
      size_t index = 0;
      zvec v1(dim.size());
      zvec v2(dim.size());
      for ( size_t E = 0; E < totE; E++ )
        {
          fbm.generate();

          v1.zero();
          while(1)
            {
              v2.zero();
              while(1)
                {
                  sums[index++] += fbm(v1) * fbm(v2);
                  if ( !v2.increment(dim) )
                      break;
                }
              if ( !v1.increment(dim) )
                  break;
            }
        }
      count++;

      if ( !(count % printInterval) )
        {
          printf("%zu\n", count);
          size_t index = 0;
          zvec v1(dim.size());
          zvec v2(dim.size());
          for ( size_t E = 0; E < totE; E++ )
            {
              double maxabserr = 0, maxrelerr = 0;
              double maxrelerrcov = 0, maxrelerrv = 0, maxabserrv = 0, maxabserrcov = 0;
              v1.zero();
              while(1)
                {
                  v2.zero();
                  while(1)
                    {
                      // cov of v1 versus v2
                      zvec dif(dim.size());
                      for ( size_t i = 0; i < dim.size(); i++ )
                          dif[i] = v1[i] - v2[i];
                      double cov = context.cov(dif);
                      double v = sums[index++] / count;

                      if ( maxabserr < fabs(v - cov) )
                        {
                          maxabserr = fabs(v - cov);
                          maxabserrv = v;
                          maxabserrcov = cov;
                        }
                      if ( fabs(cov) > 1e-15 && maxrelerr < fabs((v - cov)/cov) )
                        {
                          maxrelerr = fabs((v - cov)/cov);
                          maxrelerrv = v;
                          maxrelerrcov = cov;
                        }

                      if ( !v2.increment(dim) )
                          break;
                    }
                  if ( !v1.increment(dim) )
                      break;
                }
              printf("%zu: maxabserr=%g (%g vs cov %g), maxrelerr=%g (%g vs cov %g)\n",
                  E,
                  maxabserr, maxabserrv, maxabserrcov,
                  maxrelerr, maxrelerrv, maxrelerrcov);
            }
        }
    }
}

libfbm::PowerLawContext::PowerLawContext(double H, const zvec& dim, double Var)
  : SGPContext(dim, dim, "power." + dbl2str(H)),
    H(H), Var(Var)
{
  assert(H < 0);
}

double libfbm::PowerLawContext::cov(const zvec& dim)
{
  double r2 = 0;
  for ( size_t i = 0; i < dim.size(); i++ )
      r2 += (double)dim[i] * dim[i];
  if ( !r2 )
      return Var;
  return pow(r2, H);
}

libfbm::PLFPSField::PLFPSField(const zvec& dim, double H)
  : dim(dim), H(H)
{
  len = 1;
  for ( size_t i = 0; i < dim.size(); i++ )
    {
      assert(!(dim[i] & 1) && "dimensions must be even!");
      muls[i] = 2 * len;
      len *= dim[i];
    }
}

libfbm::PLFPSField::~PLFPSField()
{
}

bool libfbm::PLFPSField::initCache()
{
  if ( !cache.empty() )
      return true;

  double xp = -0.25 * (2 * H + (double)dim.size());

  cache.resize(len);
  double *wp = &cache[0];

  zvec p(dim.size());
  p.zero();

  long double tsum = 0;
  while(1)
    {
      double k2 = 0;
      for ( size_t i = 0; i < dim.size(); i++ )
        {
          double f;
          if ( p[i] <= dim[i] / 2 )
              f = (double)p[i] / dim[i];
          else
              f = (double)-(dim[i] - p[i]) / dim[i];
          k2 += f * f;
        }
      double mul = pow(k2, xp);
      if ( !k2 )
          mul = 0; // no constant value added?
      tsum += mul * mul;
      *wp++ = mul;
      if ( !p.increment(dim) )
          break;
    }
  double gmul = 1 / sqrt(tsum);
  for ( size_t i = 0; i < cache.size(); i++ )
      cache[i] *= gmul;

  return true;
}

void libfbm::PLFPSField::generate()
{
  generate(gaussianRandomGenerator);
}

void libfbm::PLFPSField::generate(GaussianGenerator& rng)
{
  initCache();

  if ( !Z.empty() )
    {
      if ( datap == &Z[0] )
        {
          datap++;
          return;
        }
    }

  if ( Z.empty() )
    {
      Z.resize(len * 2);
      fft.init(dim, &Z[0], FFT::forward);
    }

  datap = &Z[0];

  rng.getDouble(&Z[0], Z.size());
  for ( size_t i = 0; i < cache.size(); i++ )
    {
      Z[2 * i + 0] = cache[i] * Z[2 * i + 0];
      Z[2 * i + 1] = cache[i] * Z[2 * i + 1];
    }
  
  fft.execute();
}

void libfbm::PLFPSField::clear()
{
  std::vector<double> e;
  e.swap(Z);
  std::vector<double> e2;
  cache.swap(Z);
}

libfbm::SFMTGaussianGenerator libfbm::gaussianRandomGenerator;

libfbm::Seeder::Seeder(const char *path)
{
  fp = fopen(path, "rb");
  assert(fp);
}

libfbm::Seeder::~Seeder()
{
  fclose(fp);
}

void libfbm::Seeder::seed(uint32_t index)
{
  fseeko(fp, 0, SEEK_END);
  uint64_t len = ftello(fp);
  assert(len > 16 * size() && "entropy pool too small"); // otherwise it's nonsense what we're doing
  uint64_t pos = (uint64_t)index * size();
  pos = pos % (len - size());
  fseeko(fp, pos, SEEK_SET);
  assert(fread(buf, size(), 1, fp) == 1);
  uint32_t *sbuf = (uint32_t*)buf;
  *sbuf = index; // replace first by index
  // now we probably need to hash the tail end somehow to increase the randomness
  for ( size_t i = 1; i < size() / sizeof(uint32_t); i++ )
      sbuf[i] ^= index;
  // XXX this isn't that nice, most bits across iterations remain the same. think of something  better later
}

