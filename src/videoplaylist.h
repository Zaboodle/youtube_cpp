#pragma once

/** A class used to represent a Playlist */

class VideoPlaylist {
private:
	std::vector<Video> playlist;
public:

	void add(const Video& video);
	void remove();
	void clear();

	std::vector<Video> getVideos();
	const Video* getVideo(const std::string& videoId);

};
