/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright 2022 Blender Foundation. All rights reserved. */

#include <cstdint>

#include "BLI_hash.hh"
#include "BLI_map.hh"
#include "BLI_math_vec_types.hh"
#include "BLI_vector.hh"

#include "GPU_texture.h"

#include "VPC_texture_pool.hh"

namespace blender::viewport_compositor {

/* --------------------------------------------------------------------
 * Texture Pool Key.
 */

TexturePoolKey::TexturePoolKey(int2 size, eGPUTextureFormat format) : size(size), format(format)
{
}

TexturePoolKey::TexturePoolKey(const GPUTexture *texture)
{
  size = int2(GPU_texture_width(texture), GPU_texture_height(texture));
  format = GPU_texture_format(texture);
}

uint64_t TexturePoolKey::hash() const
{
  return get_default_hash_3(size.x, size.y, format);
}

bool operator==(const TexturePoolKey &a, const TexturePoolKey &b)
{
  return a.size == b.size && a.format == b.format;
}

/* --------------------------------------------------------------------
 * Texture Pool.
 */

GPUTexture *TexturePool::acquire(int2 size, eGPUTextureFormat format)
{
  /* Check if there is an available texture with the required specification, and one exists, return
   * it. */
  const TexturePoolKey key = TexturePoolKey(size, format);
  Vector<GPUTexture *> &available_textures = textures_.lookup_or_add_default(key);
  if (!available_textures.is_empty()) {
    return available_textures.pop_last();
  }

  /* Otherwise, allocate a new texture. */
  return allocate_texture(size, format);
}

GPUTexture *TexturePool::acquire_color(int2 size)
{
  return acquire(size, GPU_RGBA16F);
}

/* Vectors are and should be stored in RGBA textures. */
GPUTexture *TexturePool::acquire_vector(int2 size)
{
  return acquire(size, GPU_RGBA16F);
}

GPUTexture *TexturePool::acquire_float(int2 size)
{
  return acquire(size, GPU_R16F);
}

void TexturePool::release(GPUTexture *texture)
{
  textures_.lookup(TexturePoolKey(texture)).append(texture);
}

}  // namespace blender::viewport_compositor