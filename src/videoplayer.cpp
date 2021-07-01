#include "videoplayer.h"

#include <iostream>
#include <unordered_map>


std::vector<Video>::const_iterator VideoPlayer::getVideo(const std::vector<Video>& videos, const std::string& videoId) {
    return std::find_if(videos.cbegin(), videos.cend(), [videoId](const Video& obj) {return obj.getVideoId() == videoId; });
}

void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
            << std::endl;
}

void VideoPlayer::showAllVideos() {
  std::cout << "Here's a list of all available videos:" << std::endl;
  std::vector<Video> videos = mVideoLibrary.getVideos();

  for (int i = 0; i < videos.size(); i++) {
      std::string title = videos[i].getTitle();
      std::string videoId = videos[i].getVideoId();
      std::vector<std::string> tagsVector = videos[i].getTags();
      std::string tagsString;
      for (int j = 0; j < tagsVector.size(); j++) {
          if (j == 0) {
            tagsString += tagsVector[j];
          }
          else {
              tagsString += " " + tagsVector[j];
          }
          
      }
      std::cout << title << " (" << videoId << ") [" << tagsString << "]" 
          << std::endl;
  }

}

void VideoPlayer::playVideo(const std::string& videoId) {

  std::vector<Video> videos = mVideoLibrary.getVideos();

  std::vector<Video>::const_iterator video = getVideo(videos, videoId);
  
  if (video == videos.end()) {

      std::cout << "Cannot play video: Video does not exist" << std::endl;
      return;
  }

  if (mplaying.empty()) {
      mplaying = videoId;
      std::cout << "Playing video: " << video->getTitle() << std::endl;
      paused = false;
      return;
  }
  else {
      std::vector<Video>::const_iterator currentVideo = getVideo(videos, mplaying);
      std::cout << "Stopping video: " << currentVideo->getTitle() << std::endl;
      mplaying = videoId;
      std::cout << "Playing video: " << video->getTitle() << std::endl;
      paused = false;
      return;
  }
  
}

void VideoPlayer::stopVideo() {

  if (mplaying.empty()) {
      std::cout << "Cannot stop video: No video is currently playing" << std::endl;
      return;
  }
  else {
      std::vector<Video> videos = mVideoLibrary.getVideos();
      std::vector<Video>::const_iterator currentVideo = getVideo(videos, mplaying);
      mplaying = "";
      std::cout << "Stopping video: " << currentVideo->getTitle() << std::endl;
  }
  paused = false;
}

void VideoPlayer::playRandomVideo() {
  srand(time(NULL));
  int selection = std::rand()% mVideoLibrary.getVideos().size();
  Video video = mVideoLibrary.getVideos()[selection];
  playVideo(video.getVideoId());

}

void VideoPlayer::pauseVideo() {
  if (mplaying.empty()) {
      std::cout << "Cannot pause video: No video is currently playing" << std::endl;
      return;
  }
  std::vector<Video> videos = mVideoLibrary.getVideos();
  std::vector<Video>::const_iterator currentVideo = getVideo(videos, mplaying);
  if (paused) {
      std::cout << "Video already paused: " << currentVideo->getTitle() << std::endl;
      return;
  }
  std::cout << "Pausing video: " << currentVideo->getTitle() << std::endl;
  paused = true;
}

void VideoPlayer::continueVideo() {
    if (mplaying.empty()) {
        std::cout << "Cannot continue video: No video is currently playing" << std::endl;
        return;
    }
    if (!paused) {
        std::cout << "Cannot continue video: Video is not paused" << std::endl;
        return;
    }
    std::vector<Video> videos = mVideoLibrary.getVideos();
    std::vector<Video>::const_iterator currentVideo = getVideo(videos, mplaying);
    std::cout << "Continuing video: " << currentVideo->getTitle() << std::endl;
    paused = false;
}

void VideoPlayer::showPlaying() {
  if (mplaying.empty()) {
      std::cout << "No video is currently playing" << std::endl;
      return;
  }
  std::vector<Video> videos = mVideoLibrary.getVideos();
  std::vector<Video>::const_iterator currentVideo = getVideo(videos, mplaying);

  std::string title = currentVideo->getTitle();
  std::string videoId = currentVideo->getVideoId();
  std::vector<std::string> tagsVector = currentVideo->getTags();
  std::string tagsString;
  std::string pauseAddOn = "";
  for (int j = 0; j < tagsVector.size(); j++) {
      if (j == 0) {
          tagsString += tagsVector[j];
      }
      else {
          tagsString += " " + tagsVector[j];
      }

  }
  if (paused) {
      pauseAddOn = "- PAUSED";
  }
  std::cout << "Currently playing: " << title << " (" << videoId << ") [" << tagsString << "] " << pauseAddOn << std::endl;

}

void VideoPlayer::createPlaylist(const std::string& playlistName) {
  if (playlists.empty()) {
      playlists[playlistName] = VideoPlaylist();
      std::cout << "Successfully created new playlist: " << playlistName << std::endl;
      return;
  }

  std::unordered_map<std::string, VideoPlaylist>::const_iterator existing = playlists.find(playlistName);
  if (existing == playlists.cend()) {
      std::cout << "Cannot create playlist: A playlist with the same name already exists" << std::endl;
      return;
  }

  playlists[playlistName] = VideoPlaylist();
  std::cout << "Succesfully created new playlist: " << playlistName << std::endl;
}

void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {

  if (playlists.empty()) {
      std::cout << "Cannot add video to " << playlistName << ": Playlist does not exist" << std::endl;
      return;
  }

  std::unordered_map<std::string, VideoPlaylist>::const_iterator existing = playlists.find(playlistName);

  if (existing == playlists.cend()) {
      std::cout << "Cannot add video to "<< playlistName << ": Playlist does not exist" << std::endl;
      return;
  }

  std::vector<Video> videos = mVideoLibrary.getVideos();
  std::vector<Video>::const_iterator video = getVideo(videos, videoId);

  if (video == videos.end()) {
      std::cout << "Cannot add video to " << playlistName << ": Video does not exist" << std::endl;
      return;
  }
  
  VideoPlaylist playlist = existing->second;
  if (playlist.getVideo(videoId) == nullptr) {
      playlist.add(*video);
  }
}

void VideoPlayer::showAllPlaylists() {
    if (playlists.empty()) {
        std::cout << "No playlists exist yet" << std::endl;
        return;
    }
    std::cout << "Showing all playlists:" << std::endl;
    for (const auto& kv: playlists) {
        std::string name = kv.first;
        std::cout << name << std::endl;
    }
}

void VideoPlayer::showPlaylist(const std::string& playlistName) {

  if (playlists.empty()) {
      std::cout << "Cannot show playlist " << playlistName << ": Playlist does not exist" << std::endl;
      return;
  }

  std::unordered_map<std::string, VideoPlaylist>::const_iterator existing = playlists.find(playlistName);
  if (existing == playlists.end()) {
      std::cout << "Cannot show playlist " << playlistName << ": Playlist does not exist" << std::endl;
      return;
   }
  VideoPlaylist playlist = existing->second;
  std::cout << "Showing playlist: " << playlistName << std::endl;

  std::vector<Video> playlistVideos = playlist.getVideos();
  if (playlist.getVideos().empty()) {
      std::cout << "No videos here yet" << std::endl;
      return;
  }
  
  for (int i = 0; i < playlistVideos.size(); i++) {
      Video currentVideo = playlistVideos[i];
      std::string title = currentVideo.getTitle();
      std::string videoId = currentVideo.getVideoId();
      std::vector<std::string> tagsVector = currentVideo.getTags();
      std::string tagsString;
      for (int j = 0; j < tagsVector.size(); j++) {
          if (j == 0) {
              tagsString += tagsVector[j];
          }
          else {
              tagsString += " " + tagsVector[j];
          }

      }
      std::cout << title << " (" << videoId << ") [" << tagsString << "]" << std::endl;
  }
}

void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {

  if (playlists.empty()) {
      std::cout << "Cannot remove video from " << playlistName << ": Playlist does not exist" << std::endl;
      return;
  }

  std::unordered_map<std::string, VideoPlaylist>::const_iterator existing = playlists.find(playlistName);

  if (existing == playlists.end()) {
      std::cout << "Cannot remove video from " << playlistName << ": Playlist does not exist" << std::endl;
      return;
  }



  VideoPlaylist playlist = existing->second;
  std::vector<Video> videos = playlist.getVideos();

  if (videos.empty()) {
      std::cout << "Cannot remove video from " << playlistName << ": Video does not exist" << std::endl;
      return;
  }

  std::vector<Video>::const_iterator video = getVideo(videos, videoId);
  if (video == videos.cend()) {
      if (videos.empty()) {
          std::cout << "Cannot remove video from " << playlistName << ": Video does not exist" << std::endl;
          return;
      }
  }
}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {
  std::cout << "clearPlaylist needs implementation" << std::endl;
}

void VideoPlayer::deletePlaylist(const std::string& playlistName) {
  std::cout << "deletePlaylist needs implementation" << std::endl;
}

void VideoPlayer::searchVideos(const std::string& searchTerm) {
  std::cout << "searchVideos needs implementation" << std::endl;
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
  std::cout << "searchVideosWithTag needs implementation" << std::endl;
}

void VideoPlayer::flagVideo(const std::string& videoId) {
  std::cout << "flagVideo needs implementation" << std::endl;
}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& reason) {
  std::cout << "flagVideo needs implementation" << std::endl;
}

void VideoPlayer::allowVideo(const std::string& videoId) {
  std::cout << "allowVideo needs implementation" << std::endl;
}
