public class RecyclerView {

}

/***
// 线性布局  
final LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);  
linearLayoutManager.setOrientation(LinearLayoutManager.HORIZONTAL);  
mRecyclerView.setLayoutManager(linearLayoutManager);  
使用一个线性布局管理器，其方向设置横向，这样一个横向ListView样式的列表就出来了。

下面的情况可能需要调用一下
recyclerView.setHasFixedSize(true);
recyclerView.setChildDrawingOrderCallback(adapter);

TV上面item获得焦点
1.
<android.support.v7.widget.RecyclerView
android:layout_width="match_parent"
android:layout_height="match_parent"
android:focusable="true"/>
2.
item.xml
加上
android:focusable="true"
默认第一个item被选中
1.
mRecyclerView.setOnFocusChangeListener(new OnFocusChangeListener() {  
    @Override  
    public void onFocusChange(View v, boolean hasFocus) { 
        if(hasFocus){  
            if(mRecyclerView.getChildCount()>0){  
                mRecyclerView.getChildAt(0).requestFocus();  
            }  
        }  
    }  
});
2.
holder.itemView.setFocusable(true);  

TV上面item获得焦点后当前item能够放大，失去焦点时恢复成原样
1.
在onBindViewHolder方法中实现下面代码
RecyclerViewHolder viewHolder = (RecyclerViewHolder)holder;
viewHolder.itemView.setOnFocusChangeListener(
new View.OnFocusChangeListener(){
    public void onFocusChange(View view, boolean hasFocus){
        if(hasFocus){
            view.setBackgroundColor(getColor(R.color.darkviolet));
            ViewCompat.animate(view)
            .scaleX(1.2f).scaleY(1.2f).translationZ(1).start();
        } else {
            view.setBackgroundColor(getColor(R.color.limegreen));
            ViewCompat.animate(view)
            .scaleX(1.0f).scaleY(1.0f).start();
        }

        recyclerView.invalidate();// 关键代码
    }
});
2.
覆写下面的方法（作用是：选中的item最后更新，这样选中的item才会在其他item的上面）
View focusedChildView;
int focusViewIndex;
int focusid;
public int onGetChildDrawingOrder(int childCount, int i){
    focusedChildView = recyclerView.getFocusedChild();
    focusViewIndex = recyclerView.indexOfChild(focusedChildView);
    if(focusViewIndex == -1){
        return i;
    }
    if(focusViewIndex == i){
        focusid = i;
        return childCount - 1;
    }else if(i == childCount - 1){
        return focusid;
    }else{
        return i;
    }
}

在Adapter中调用notifyDataSetChanged()方法后会调用onBindViewHolder()方法的可见item数量的次数。意思就是如可见item有10个，那么调用notifyDataSetChanged()方法后会调用10次onBindViewHolder()方法。

RV占据焦点，RV的子View没有获取焦点
需要在RV获取焦点后手动设置下焦点：
void focusRecyclerView(boolean hasFocus) {
    if (hasFocus) {
        focusChild(false);
    }
}

void focusChild(boolean isInit) {  
    if (mRecyclerView.getChildCount() > 0) {
        mRecyclerView.getChildAt(0).requestFocus();
    }
    if (isInit) {
        mAct.setSelectPos(0);
    }
}

选中的背景问题 
在SwappingHolder的子类中复写如下方法：
public void setSelectionModeBackgroundDrawable(
    Drawable selectionModeBackgroundDrawable) {  supper.setSelectionModeBackgroundDrawable(mAct.getResources().getDrawable(R.drawable.selector_list_item_shadow));
}

下面的没有验证过
setSelection()方法，跳转到最后几个Item的问题
有个问题就是，如果总共有15张图片，最后一页展示4个图片，那么跳转到pos大于11的图片时候，焦点却在pos==11的图片位置上。所以这个时候需要手动设置下。
void fouceSelected() {
    if (mAct.getSelectPos()  <= linearLayoutManager.findLastVisibleItemPosition()
    && mAct.getSelectPos() >=linearLayoutManager.findFirstVisibleItemPosition()) {
            linearLayoutManager.findViewByPosition(mAct.getSelectPos()).requestFocus();

            int focusPos = mRecyclerView.getChildCount() + mAct.getSelectPos() - linearLayoutManager.findLastVisibleItemPosition() - 1;
            mRecyclerView.getChildAt(focusPos).requestFocus();
        }
}
获取RV当前显示的第一个控件的位置和最后一个控件显示的位置来计算出当前应该获取焦点的View的位置，把焦点交给它。

自定义LayoutManager的基础知识点
onLayoutChildren()，这个方法是在RecyclerView初始化的时候回调的，并且会回调两次.
在每一次的滑动事件中，都会完整的经过dy修正、子view填充、子view移动、子view回收四个过程.
scrollVerticallyBy的实现步骤列一下：
1.偏移值dy修正
2.填充子view
3.偏移子view
4.回收子view

detachAndScrapView和removeAndRecycleView的区别
detachAndScrapView:
在需要刷新的时候使用，子view临时保存在Recycler中的mAttachedScrap中，其后马上用到；
removeAndRecycleView:
在子view移出屏幕回收时使用，保存在Recycler中的mCachedViews中，
mAttachedScrap和mCachedViews都是一个ArrayList类型的集合。
其次，detachAndScrapView不会引起重新布局，因此是轻量级的，而removeAndRecycleView会引起重新布局。
detachAndScrapAttachedViews:
RecyclerView初始化的时候onLayoutChildren会调用两次，第一次的时候屏幕上已经填充好子view了，第二次到来的时候又会重新调用一次fill填充子view，因此fill之前先调用轻量级的detachAndScrapAttachedViews把子view都给移除掉，临时保存在一个集合里，然后进入fill的时候会从这个集合取出来重新添加到RecyclerView中

在自定义LayoutManager中，我们需要重写onLayoutChildren方法，这个方法会在初始化或者Adapter数据集更新时回调，在这方法里面，需要做以下事情：
进行布局之前，我们需要调用detachAndScrapAttachedViews方法把屏幕中的Items都分离出来，内部调整好位置和数据后，再把它添加回去(如果需要的话)；
分离了之后，我们就要想办法把它们再添加回去了，所以需要通过addView方法来添加，那这些View在哪里得到呢？ 我们需要调用 Recycler的getViewForPosition(int position) 方法来获取；
获取到Item并重新添加了之后，我们还需要对它进行测量，这时候可以调用measureChild或measureChildWithMargins方法，两者的区别我们已经了解过了，相信同学们都能根据需求选择更合适的方法；
在测量完还需要做什么呢？ 没错，就是布局了，我们也是根据需求来决定使用layoutDecorated还是layoutDecoratedWithMargins方法；
在自定义ViewGroup中，layout完就可以运行看效果了，但在LayoutManager还有一件非常重要的事情，就是回收了，我们在layout之后，还要把一些不再需要的Items回收，以保证滑动的流畅度；

列表向下滚动dy为正，列表向上滚动dy为负
*/