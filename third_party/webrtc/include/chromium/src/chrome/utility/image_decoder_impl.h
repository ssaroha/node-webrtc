// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMAGE_DECODER_IMPL_H_
#define CHROME_UTILITY_IMAGE_DECODER_IMPL_H_

#include "base/macros.h"
#include "chrome/common/image_decoder.mojom.h"

class ImageDecoderImpl : public mojom::ImageDecoder {
 public:
  ImageDecoderImpl();
  explicit ImageDecoderImpl(int64_t max_message_size);
  ~ImageDecoderImpl() override;

  // Overridden from mojom::ImageDecoder:
  void DecodeImage(
      mojo::Array<uint8_t> encoded_data,
      mojom::ImageCodec codec,
      bool shrink_to_fit,
      const DecodeImageCallback& callback) override;

 private:
  int64_t max_message_size_;

  DISALLOW_COPY_AND_ASSIGN(ImageDecoderImpl);
};

#endif  // CHROME_UTILITY_IMAGE_DECODER_IMPL_H_
