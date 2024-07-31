# FlickFix

FlickFix is a free video editing desktop application emphasizing simplicity and ease of use. FlickFix aims to offer an intuitive and user-friendly video editing experience by streamlining the platform to include only the essential features. This approach allows even novice editors to edit videos without feeling intimidated or overwhelmed. Users can access several common features such as video trimming, intro and outro creation, and audio dubbing, all within a user-friendly GUI.
## Build/Deploy

### How to run FlickFix:

Get FFmpeg
1. Download FFmpeg: https://www.gyan.dev/ffmpeg/builds/packages/ffmpeg-6.0-essentials_build.zip
2. Extract and move contents to a new directory C:\ffmpeg
3. Go to 'Edit the system environment variables' in Windows.
4. Click on 'Advanced system settings'.
5. Under 'System variables', click 'Path'.
6. Click 'Edit' and 'New'.
7. Add the path C:\ffmpeg\bin

Get K-Lite Codec Pack
1. Download K-Lite Codec Basic: https://codecguide.com/download_k-lite_codec_pack_basic.htm
2. Install with default options.

Install FlickFix
1. Download installer: https://github.com/mtweigel/CSU-Senior-Project/blob/master/FlickFix.exe
2. Install and Run

Note: This is being detected as unsafe by Windows Defender, so you will need to temporarily disable real time virus detection to install. Sketchy, I know but this happens because I have not purchased a digital certificate. 

You should be able to run FlickFix at this point. If not, please contact me with your issue.

<br>

### How to build FlickFix:
If you want to build the project yourself, this is how you can do that:
1. Download FFmpeg and K-Lite Codec.
2. Download Qt Creator 5.15.2.
3. Download all of the files found in src: https://github.com/mtweigel/CSU-Senior-Project/tree/master/src
4. Place them together in a single folder called FlickFix.
5. In Qt Creator, open the file FlickFix.pro.
6. Build and Run.

## Testing

Test Case Document: https://docs.google.com/spreadsheets/d/1AKGEy2gAnmjuKqQM1qJmJLxnYHLELVpD3hXlkFgVaD0/edit?usp=sharing

