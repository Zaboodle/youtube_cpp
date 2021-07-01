#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include "video.h"
#include "videoplaylist.h"

void VideoPlaylist::add(const Video& video) {
    playlist.emplace_back(video);
}

void VideoPlaylist::clear() {

}

void VideoPlaylist::remove() {

}

std::vector<Video> VideoPlaylist::getVideos() {
    return playlist;
}

const Video* VideoPlaylist::getVideo(const std::string& videoId) {
    for (int i = 0; i < playlist.size(); i++) {
        if (playlist[i].getVideoId() == videoId) {
            return &playlist[i];
        }
    }
    return nullptr;
}
