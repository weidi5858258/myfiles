import android.app.AppGlobals;
import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Point;
import android.os.RemoteException;
import android.provider.Settings;
import android.util.DisplayMetrics;
import android.util.SparseArray;

/**
 * 主要用来获取一些在UI中所使用到的标准常量，像超时、尺寸、距离
 */
public class ViewConfiguration {
    /**
     * 定义了水平滚动条的宽度和垂直滚动条的高度，单位是dip
     */
    private static final int SCROLL_BAR_SIZE = 10;

    /**
     * 滚动条褪去所需要经历的时间，单位：milliseconds
     */
    private static final int SCROLL_BAR_FADE_DURATION = 250;

    /**
     * 滚动条褪去之前的默认时间延迟，单位：milliseconds
     */
    private static final int SCROLL_BAR_DEFAULT_DELAY = 300;

    /**
     * 定义褪去边缘的长度，单位：dip
     */
    private static final int FADING_EDGE_LENGTH = 12;

    /**
     * 按下状态在子控件上的持续时间，单位：milliseconds
     */
    private static final int PRESSED_STATE_DURATION = 64;

    /**
     * 定义一个按下状态转变成长按状态所需要持续的时间，单位：milliseconds
     */
    private static final int DEFAULT_LONG_PRESS_TIMEOUT = 500;

    /**
     * 定义连续重复按键间的时间延迟，单位：milliseconds
     */
    private static final int KEY_REPEAT_DELAY = 50;

    /**
     * 如果用户需要触发全局对话框，例如：关机，锁屏等，需要按下按钮所持续的事件，单位：milliseconds
     */
    private static final int GLOBAL_ACTIONS_KEY_TIMEOUT = 500;

    /**
     * 定义一个触摸事件是点击还是滚动的事件间隔，如果在这个事件内没有移动，就认为这是一个点击，否则就是滚动，单位：milliseconds
     */
    private static final int TAP_TIMEOUT = 180;

    /**
     * Defines the duration in milliseconds we will wait to see if a touch event
     * is a jump tap. If the user does not complete the jump tap within this interval, it is
     * considered to be a tap.
     */
    private static final int JUMP_TAP_TIMEOUT = 500;

    /**
     * 定义双击的时间间隔，如果在这个时间内，就认为是双击
     */
    private static final int DOUBLE_TAP_TIMEOUT = 300;

    /**
     * 定义双击最小的时间间隔
     */
    private static final int DOUBLE_TAP_MIN_TIME = 40;

    /**
     * 定义一个触摸板触摸到释放可认为是一个点击事件而不是一个触摸移动手势的最大时间，
     * 也就是说在这个时间内进行一次触摸和释放操作就可以认为是一次点击事件，单位：milliseconds
     */
    private static final int HOVER_TAP_TIMEOUT = 150;

    /**
     * 定义一个触摸板在触摸释放之前可以移动的最大距离，
     * 如果在这个距离之内就可以认为是一个点击事件，否则就是一个移动手势，单位：pixels
     */
    private static final int HOVER_TAP_SLOP = 20;

    /**
     * 定义响应显示缩放控制的时间
     */
    private static final int ZOOM_CONTROLS_TIMEOUT = 3000;

    /**
     * Inset in dips to look for touchable content when the user touches the edge of the screen
     */
    private static final int EDGE_SLOP = 12;

    /**
     * 如果我们认为用户正在滚动，这里定义一个触摸事件可以滚动的距离，单位：dips
     * 注意：这个值在这里定义只是作为那些没有提供上下文Context来决定密度和配置相关值的应用程序的一个备用值。
     */
    private static final int TOUCH_SLOP = 8;

    /**
     * 定义双击事件之间可以移动的距离，单位：dips
     */
    private static final int DOUBLE_TAP_TOUCH_SLOP = TOUCH_SLOP;

    /**
     * 定义用户尝试翻页滚动的触摸移动距离，单位：dips
     *
     * 注意：这个值在这里定义只是作为那些没有提供上下文Context来决定密度和配置相关值的应用程序的一个备用值。
     *
     */
    private static final int PAGING_TOUCH_SLOP = TOUCH_SLOP * 2;

    /**
     * 定义第一次点击和第二次点击可以认为是一次双击之间的距离。单位：dips
     */
    private static final int DOUBLE_TAP_SLOP = 100;

    /**
     * Distance in dips a touch needs to be outside of a window's bounds for it to
     * count as outside for purposes of dismissing the window.
     */
    private static final int WINDOW_TOUCH_SLOP = 16;

    /**
     * 一个fling最小的速度，单位：dips/s
     */
    private static final int MINIMUM_FLING_VELOCITY = 50;

    /**
     * 一个fling最大的速度，单位：dips/s
     */
    private static final int MAXIMUM_FLING_VELOCITY = 8000;

    /**
     * 分发一个重复访问事件的延迟事件，单位：milliseconds
     */
    private static final long SEND_RECURRING_ACCESSIBILITY_EVENTS_INTERVAL_MILLIS = 100;

    /**
     * The maximum size of View's drawing cache, expressed in bytes. This size
     * should be at least equal to the size of the screen in ARGB888 format.
     */
    @Deprecated
    private static final int MAXIMUM_DRAWING_CACHE_SIZE = 480 * 800 * 4; // ARGB8888

    /**
     * 滚动和滑动的摩擦系数
     */
    private static final float SCROLL_FRICTION = 0.015f;

    /**
     * Max distance in dips to overscroll for edge effects
     */
    private static final int OVERSCROLL_DISTANCE = 0;

    /**
     * Max distance in dips to overfling for edge effects
     */
    private static final int OVERFLING_DISTANCE = 6;

    private final int mEdgeSlop;
    private final int mFadingEdgeLength;
    private final int mMinimumFlingVelocity;
    private final int mMaximumFlingVelocity;
    private final int mScrollbarSize;
    private final int mTouchSlop;
    private final int mDoubleTapTouchSlop;
    private final int mPagingTouchSlop;
    private final int mDoubleTapSlop;
    private final int mWindowTouchSlop;
    private final int mMaximumDrawingCacheSize;
    private final int mOverscrollDistance;
    private final int mOverflingDistance;
    private final boolean mFadingMarqueeEnabled;

    private boolean sHasPermanentMenuKey;
    private boolean sHasPermanentMenuKeySet;

    static final SparseArray<ViewConfiguration> sConfigurations =
            new SparseArray<ViewConfiguration>(2);

    /**
     * 这个方法被废除了，使用ViewConfiguration.get(Context)}替代
     */
    @Deprecated
    public ViewConfiguration() {
        mEdgeSlop = EDGE_SLOP;
        mFadingEdgeLength = FADING_EDGE_LENGTH;
        mMinimumFlingVelocity = MINIMUM_FLING_VELOCITY;
        mMaximumFlingVelocity = MAXIMUM_FLING_VELOCITY;
        mScrollbarSize = SCROLL_BAR_SIZE;
        mTouchSlop = TOUCH_SLOP;
        mDoubleTapTouchSlop = DOUBLE_TAP_TOUCH_SLOP;
        mPagingTouchSlop = PAGING_TOUCH_SLOP;
        mDoubleTapSlop = DOUBLE_TAP_SLOP;
        mWindowTouchSlop = WINDOW_TOUCH_SLOP;
        //noinspection deprecation
        mMaximumDrawingCacheSize = MAXIMUM_DRAWING_CACHE_SIZE;
        mOverscrollDistance = OVERSCROLL_DISTANCE;
        mOverflingDistance = OVERFLING_DISTANCE;
        mFadingMarqueeEnabled = true;
    }

    /**
     * 使用给定的context来创建一个新的配置。这个配置依赖于context里面不同的参数，例如显示的尺寸或者密度
     * @param context 用来初始化这个view配置的应用上下文环境
     *
     * @see #get(android.content.Context)
     * @see android.util.DisplayMetrics
     */
    private ViewConfiguration(Context context) {
        final Resources res = context.getResources();
        final DisplayMetrics metrics = res.getDisplayMetrics();
        final Configuration config = res.getConfiguration();
        final float density = metrics.density;
        final float sizeAndDensity;
        if (config.isLayoutSizeAtLeast(Configuration.SCREENLAYOUT_SIZE_XLARGE)) {
            sizeAndDensity = density * 1.5f;
        } else {
            sizeAndDensity = density;
        }

        mEdgeSlop = (int) (sizeAndDensity * EDGE_SLOP + 0.5f);
        mFadingEdgeLength = (int) (sizeAndDensity * FADING_EDGE_LENGTH + 0.5f);
        mMinimumFlingVelocity = (int) (density * MINIMUM_FLING_VELOCITY + 0.5f);
        mMaximumFlingVelocity = (int) (density * MAXIMUM_FLING_VELOCITY + 0.5f);
        mScrollbarSize = (int) (density * SCROLL_BAR_SIZE + 0.5f);
        mDoubleTapSlop = (int) (sizeAndDensity * DOUBLE_TAP_SLOP + 0.5f);
        mWindowTouchSlop = (int) (sizeAndDensity * WINDOW_TOUCH_SLOP + 0.5f);

        // Size of the screen in bytes, in ARGB_8888 format
        final WindowManager win = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
        final Display display = win.getDefaultDisplay();
        final Point size = new Point();
        display.getRealSize(size);
        mMaximumDrawingCacheSize = 4 * size.x * size.y;

        mOverscrollDistance = (int) (sizeAndDensity * OVERSCROLL_DISTANCE + 0.5f);
        mOverflingDistance = (int) (sizeAndDensity * OVERFLING_DISTANCE + 0.5f);

        if (!sHasPermanentMenuKeySet) {
            IWindowManager wm = WindowManagerGlobal.getWindowManagerService();
            try {
                sHasPermanentMenuKey = !wm.hasNavigationBar();
                sHasPermanentMenuKeySet = true;
            } catch (RemoteException ex) {
                sHasPermanentMenuKey = false;
            }
        }

        mFadingMarqueeEnabled = res.getBoolean(
                com.android.internal.R.bool.config_ui_enableFadingMarquee);
        mTouchSlop = res.getDimensionPixelSize(
                com.android.internal.R.dimen.config_viewConfigurationTouchSlop);
        mPagingTouchSlop = mTouchSlop * 2;

        mDoubleTapTouchSlop = mTouchSlop;
    }

    /**
     * 跟上面一个函数一样，只不过上面一个是创建一个ViewConfiguration对象，这里是直接通过这个静态方法返回一个对象
     */
    public static ViewConfiguration get(Context context) {
        final DisplayMetrics metrics = context.getResources().getDisplayMetrics();
        final int density = (int) (100.0f * metrics.density);

        ViewConfiguration configuration = sConfigurations.get(density);
        if (configuration == null) {
            configuration = new ViewConfiguration(context);
            sConfigurations.put(density, configuration);
        }

        return configuration;
    }

    /**
     * @return 获取水平滚动条的宽带和垂直滚动条的高度
     *
     * 这个函数被废除，使用getScaledScrollBarSize()来代替
     */
    @Deprecated
    public static int getScrollBarSize() {
        return SCROLL_BAR_SIZE;
    }

    /**
     * @return 获取水平滚动条的宽带和垂直滚动条的高度
     */
    public int getScaledScrollBarSize() {
        return mScrollbarSize;
    }

    /**
     * @return 滚动条褪去的持续时间
     */
    public static int getScrollBarFadeDuration() {
        return SCROLL_BAR_FADE_DURATION;
    }

    /**
     * @return 滚动条褪去的延迟时间
     */
    public static int getScrollDefaultDelay() {
        return SCROLL_BAR_DEFAULT_DELAY;
    }

    /**
     * @return 褪去边缘的长度
     *
     * 这个方法已经废弃，用getScaledFadingEdgeLength()替代.
     */
    @Deprecated
    public static int getFadingEdgeLength() {
        return FADING_EDGE_LENGTH;
    }

    /**
     * @return 褪去边缘的长度，单位：pixels
     */
    public int getScaledFadingEdgeLength() {
        return mFadingEdgeLength;
    }

    /**
     * @return 在子控件上按住状态的持续时间
     */
    public static int getPressedStateDuration() {
        return PRESSED_STATE_DURATION;
    }

    /**
     * @return 按住状态转变为长按状态需要的时间
     */
    public static int getLongPressTimeout() {
        return AppGlobals.getIntCoreSetting(Settings.Secure.LONG_PRESS_TIMEOUT,
                DEFAULT_LONG_PRESS_TIMEOUT);
    }

    /**
     * @return 重新按键时间
     */
    public static int getKeyRepeatTimeout() {
        return getLongPressTimeout();
    }

    /**
     * @return 重复按键延迟时间
     */
    public static int getKeyRepeatDelay() {
        return KEY_REPEAT_DELAY;
    }

    /**
     * @return 判断用户是单击还是滚动的时间，在这个时间内没有移动则是单击，否则是滚动
     */
    public static int getTapTimeout() {
        return TAP_TIMEOUT;
    }

    /**
     * @return the duration in milliseconds we will wait to see if a touch event
     * is a jump tap. If the user does not move within this interval, it is
     * considered to be a tap.
     */
    public static int getJumpTapTimeout() {
        return JUMP_TAP_TIMEOUT;
    }

    /**
     * @return 得到双击间隔时间，在这个时间内，则是双击，否则就是单击
     */
    public static int getDoubleTapTimeout() {
        return DOUBLE_TAP_TIMEOUT;
    }

    /**
     * @return the minimum duration in milliseconds between the first tap's
     * up event and the second tap's down event for an interaction to be considered a
     * double-tap.
     *
     * @hide
     */
    public static int getDoubleTapMinTime() {
        return DOUBLE_TAP_MIN_TIME;
    }

    /**
     * @return the maximum duration in milliseconds between a touch pad
     * touch and release for a given touch to be considered a tap (click) as
     * opposed to a hover movement gesture.
     * @hide
     */
    public static int getHoverTapTimeout() {
        return HOVER_TAP_TIMEOUT;
    }

    /**
     * @return the maximum distance in pixels that a touch pad touch can move
     * before being released for it to be considered a tap (click) as opposed
     * to a hover movement gesture.
     * @hide
     */
    public static int getHoverTapSlop() {
        return HOVER_TAP_SLOP;
    }

    /**
     * @return Inset in dips to look for touchable content when the user touches the edge of the
     *         screen
     *
     * @deprecated Use {@link #getScaledEdgeSlop()} instead.
     */
    @Deprecated
    public static int getEdgeSlop() {
        return EDGE_SLOP;
    }

    /**
     * @return Inset in pixels to look for touchable content when the user touches the edge of the
     *         screen
     */
    public int getScaledEdgeSlop() {
        return mEdgeSlop;
    }

    /**
     * @return Distance in dips a touch can wander before we think the user is scrolling
     *
     * @deprecated Use {@link #getScaledTouchSlop()} instead.
     */
    @Deprecated
    public static int getTouchSlop() {
        return TOUCH_SLOP;
    }

    /**
     * @return Distance in pixels a touch can wander before we think the user is scrolling
     */
    public int getScaledTouchSlop() {
        return mTouchSlop;
    }

    /**
     * @return Distance in pixels the first touch can wander before we do not consider this a
     * potential double tap event
     * @hide
     */
    public int getScaledDoubleTapTouchSlop() {
        return mDoubleTapTouchSlop;
    }

    /**
     * @return Distance in pixels a touch can wander before we think the user is scrolling a full
     * page
     */
    public int getScaledPagingTouchSlop() {
        return mPagingTouchSlop;
    }

    /**
     * @return Distance in dips between the first touch and second touch to still be
     *         considered a double tap
     * @deprecated Use {@link #getScaledDoubleTapSlop()} instead.
     * @hide The only client of this should be GestureDetector, which needs this
     *       for clients that still use its deprecated constructor.
     */
    @Deprecated
    public static int getDoubleTapSlop() {
        return DOUBLE_TAP_SLOP;
    }

    /**
     * @return Distance in pixels between the first touch and second touch to still be
     *         considered a double tap
     */
    public int getScaledDoubleTapSlop() {
        return mDoubleTapSlop;
    }

    /**
     * Interval for dispatching a recurring accessibility event in milliseconds.
     * This interval guarantees that a recurring event will be send at most once
     * during the {@link #getSendRecurringAccessibilityEventsInterval()} time frame.
     *
     * @return The delay in milliseconds.
     *
     * @hide
     */
    public static long getSendRecurringAccessibilityEventsInterval() {
        return SEND_RECURRING_ACCESSIBILITY_EVENTS_INTERVAL_MILLIS;
    }

    /**
     * @return Distance in dips a touch must be outside the bounds of a window for it
     * to be counted as outside the window for purposes of dismissing that
     * window.
     *
     * @deprecated Use {@link #getScaledWindowTouchSlop()} instead.
     */
    @Deprecated
    public static int getWindowTouchSlop() {
        return WINDOW_TOUCH_SLOP;
    }

    /**
     * @return Distance in pixels a touch must be outside the bounds of a window for it
     * to be counted as outside the window for purposes of dismissing that window.
     */
    public int getScaledWindowTouchSlop() {
        return mWindowTouchSlop;
    }

    /**
     * @return Minimum velocity to initiate a fling, as measured in dips per second.
     *
     * @deprecated Use {@link #getScaledMinimumFlingVelocity()} instead.
     */
    @Deprecated
    public static int getMinimumFlingVelocity() {
        return MINIMUM_FLING_VELOCITY;
    }

    /**
     * @return 得到滑动的最小速度, 以像素/每秒来进行计算
     */
    public int getScaledMinimumFlingVelocity() {
        return mMinimumFlingVelocity;
    }

    /**
     * @return Maximum velocity to initiate a fling, as measured in dips per second.
     *
     * @deprecated Use {@link #getScaledMaximumFlingVelocity()} instead.
     */
    @Deprecated
    public static int getMaximumFlingVelocity() {
        return MAXIMUM_FLING_VELOCITY;
    }

    /**
     * @return 得到滑动的最大速度, 以像素/每秒来进行计算
     */
    public int getScaledMaximumFlingVelocity() {
        return mMaximumFlingVelocity;
    }

    /**
     * The maximum drawing cache size expressed in bytes.
     *
     * @return the maximum size of View's drawing cache expressed in bytes
     *
     * @deprecated Use {@link #getScaledMaximumDrawingCacheSize()} instead.
     */
    @Deprecated
    public static int getMaximumDrawingCacheSize() {
        //noinspection deprecation
        return MAXIMUM_DRAWING_CACHE_SIZE;
    }

    /**
     * The maximum drawing cache size expressed in bytes.
     *
     * @return the maximum size of View's drawing cache expressed in bytes
     */
    public int getScaledMaximumDrawingCacheSize() {
        return mMaximumDrawingCacheSize;
    }

    /**
     * @return The maximum distance a View should overscroll by when showing edge effects (in
     * pixels).
     */
    public int getScaledOverscrollDistance() {
        return mOverscrollDistance;
    }

    /**
     * @return The maximum distance a View should overfling by when showing edge effects (in
     * pixels).
     */
    public int getScaledOverflingDistance() {
        return mOverflingDistance;
    }

    /**
     * The amount of time that the zoom controls should be
     * displayed on the screen expressed in milliseconds.
     *
     * @return the time the zoom controls should be visible expressed
     * in milliseconds.
     */
    public static long getZoomControlsTimeout() {
        return ZOOM_CONTROLS_TIMEOUT;
    }

    /**
     * The amount of time a user needs to press the relevant key to bring up
     * the global actions dialog.
     *
     * @return how long a user needs to press the relevant key to bring up
     *   the global actions dialog.
     */
    public static long getGlobalActionKeyTimeout() {
        return GLOBAL_ACTIONS_KEY_TIMEOUT;
    }

    /**
     * The amount of friction applied to scrolls and flings.
     *
     * @return A scalar dimensionless value representing the coefficient of
     *         friction.
     */
    public static float getScrollFriction() {
        return SCROLL_FRICTION;
    }

    /**
     * Report if the device has a permanent menu key available to the user.
     *
     * <p>As of Android 3.0, devices may not have a permanent menu key available.
     * Apps should use the action bar to present menu options to users.
     * However, there are some apps where the action bar is inappropriate
     * or undesirable. This method may be used to detect if a menu key is present.
     * If not, applications should provide another on-screen affordance to access
     * functionality.
     *
     * @return true if a permanent menu key is present, false otherwise.
     */
    public boolean hasPermanentMenuKey() {
        return sHasPermanentMenuKey;
    }

    /**
     * @hide
     * @return Whether or not marquee should use fading edges.
     */
    public boolean isFadingMarqueeEnabled() {
        return mFadingMarqueeEnabled;
    }
}