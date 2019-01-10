public class exoplayer{

}

/***
library/core/src/main/java/com/google/android/exoplayer2/upstream
一个视频的相关信息
com.google.android.exoplayer2.upstream.DataSpec
com.google.android.exoplayer2.upstream.DataSource
com.google.android.exoplayer2.upstream.HttpDataSource
com.google.android.exoplayer2.upstream.FileDataSource
FileDataSource:
一个视频还剩下多少长度没有播放
private long bytesRemaining;
private @Nullable RandomAccessFile file;
file = new RandomAccessFile(dataSpec.uri.getPath(), "r");
file.seek(dataSpec.position);
int bytesRead;
try {
	bytesRead = file.read(
		buffer, offset, (int) Math.min(bytesRemaining, readLength));
} catch (IOException e) {
	throw new FileDataSourceException(e);
}

com.google.android.exoplayer2.upstream.Loader
	public void run():
		loadable.load();
		只创建过一次
		loadable: com.google.android.exoplayer2.upstream.ParsingLoadable
		以后都是创建这个对象进行的操作(不断的在创建对象)
		loadable: com.google.android.exoplayer2.source.chunk.ContainerMediaChunk
com.google.android.exoplayer2.source.chunk.ContainerMediaChunk
	public final void load():
		dataSource.open(loadDataSpec)
		dataSource: com.google.android.exoplayer2.upstream.StatsDataSource
com.google.android.exoplayer2.upstream.StatsDataSource
	public long open(DataSpec dataSpec):
		long availableBytes = dataSource.open(dataSpec);
		dataSource: com.google.android.exoplayer2.upstream.cache.CacheDataSource
com.google.android.exoplayer2.upstream.cache.CacheDataSource
	public long open(DataSpec dataSpec):
		openNextSource(false);
			long resolvedLength = nextDataSource.open(nextDataSpec);
			nextDataSource: com.google.android.exoplayer2.upstream.DefaultHttpDataSource
com.google.android.exoplayer2.upstream.DefaultHttpDataSource
	public long open(DataSpec dataSpec):
调用open方法之后,返回的值是还剩下多少视频内容的长度没有播放(不是指时间)

com.google.android.exoplayer2.source.chunk.ContainerMediaChunk
	extractorWrapper: com.google.android.exoplayer2.source.chunk.ChunkExtractorWrapper
	dataSource: com.google.android.exoplayer2.upstream.StatsDataSource
	extractorWrapper.extractor: com.google.android.exoplayer2.extractor.mp4.FragmentedMp4Extractor

com.google.android.exoplayer2.extractor.DefaultExtractorInput
	dataSource: com.google.android.exoplayer2.upstream.StatsDataSource
com.google.android.exoplayer2.source.chunk.ContainerMediaChunk
	dataSource: com.google.android.exoplayer2.upstream.StatsDataSource
com.google.android.exoplayer2.extractor.DefaultExtractorInput
	dataSource: com.google.android.exoplayer2.upstream.StatsDataSource
com.google.android.exoplayer2.upstream.StatsDataSource
	dataSource: com.google.android.exoplayer2.upstream.cache.CacheDataSource
com.google.android.exoplayer2.upstream.cache.CacheDataSource
	currentDataSource: com.google.android.exoplayer2.upstream.DefaultDataSource
com.google.android.exoplayer2.upstream.DefaultDataSource
	dataSource: com.google.android.exoplayer2.upstream.DefaultHttpDataSource


com.google.android.exoplayer2.source.smoothstreaming.SsMediaSource
	manifestLoader = new Loader("Loader:Manifest");
	manifestLoader.startLoading(
            loadable, this, 
            loadErrorHandlingPolicy.getMinimumLoadableRetryCount(loadable.type));
com.google.android.exoplayer2.source.chunk.ChunkSampleStream
	loader = new Loader("Loader:ChunkSampleStream");// 158
	loader.startLoading(
            loadable, this, 
            loadErrorHandlingPolicy.getMinimumLoadableRetryCount(loadable.type));

















https://playready.directtaps.net/smoothstreaming/SSWSS720H264/SuperSpeedway_720.ism/QualityLevels(230000)/Fragments(video=0)
*/