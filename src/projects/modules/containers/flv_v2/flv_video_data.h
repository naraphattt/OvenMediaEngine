//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2025 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include <modules/bitstream/h264/h264_decoder_configuration_record.h>
#include <modules/bitstream/h265/h265_decoder_configuration_record.h>
#include <modules/rtmp_v2/amf0/amf_document.h>

#include "./flv_common.h"

namespace modules
{
	namespace flv
	{
		struct VideoData : public CommonData
		{
			VideoData(VideoFrameType video_frame_type, bool from_ex_header)
				: CommonData(from_ex_header),
				  video_frame_type(video_frame_type)
			{
			}

			const VideoFrameType video_frame_type;

			// Available only when `_is_ex_header == false` (Legacy AVC)
			std::optional<VideoCodecId> video_codec_id;

			// Available only when `_is_ex_header == true` (E-RTMP)
			std::optional<VideoCommand> video_command;
			std::optional<uint24_t> video_timestamp_nano_offset;
			std::optional<VideoFourCc> video_fourcc;
			std::optional<rtmp::AmfDocument> video_metadata;
			std::shared_ptr<AVCDecoderConfigurationRecord> avc_header;
			std::shared_ptr<const ov::Data> avc_header_data;
			std::shared_ptr<HEVCDecoderConfigurationRecord> hevc_header;
			// This is used to store the data to re-parse `H265DecoderConfigurationRecord`
			// when receiving `cmn::PacketType::SEQUENCE_HEADER`
			// in `MediaRouterNormalize::ProcessH265HVCCStream()`.
			// It can be improved to use what is parsed here later.
			std::shared_ptr<const ov::Data> hevc_header_data;

			// Available both in legacy and E-RTMP
			VideoPacketType video_packet_type;
			int24_t composition_time_offset;

			bool IsKeyFrame() const
			{
				return video_frame_type == VideoFrameType::KeyFrame;
			}
		};
	}  // namespace flv
}  // namespace modules
