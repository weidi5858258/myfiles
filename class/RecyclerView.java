public class RecyclerView{

}

/***
你想要控制其显示的方式，请通过布局管理器LayoutManager
你想要控制Item间的间隔（可绘制），请通过ItemDecoration
你想要控制Item增删的动画，请通过ItemAnimator
mRecyclerView = findView(R.id.id_recyclerview);
//设置布局管理器
mRecyclerView.setLayoutManager(layout);
//设置adapter
mRecyclerView.setAdapter(adapter)
//设置Item增加、移除动画
mRecyclerView.setItemAnimator(
new DefaultItemAnimator());
//添加分割线
mRecyclerView.addItemDecoration(
new DividerItemDecoration(
                getActivity(), 
                DividerItemDecoration.HORIZONTAL_LIST));

ItemDecoration（DividerItemDecoration实现类）
我们可以通过该方法添加分割线： 
mRecyclerView.addItemDecoration() 
该方法的参数为RecyclerView.ItemDecoration，
该类为抽象类，官方目前并没有提供默认的实现类。
当我们调用mRecyclerView.addItemDecoration()
方法添加decoration的时候，RecyclerView在绘制的时候，
去会绘制decorator，即调用该类的onDraw和onDrawOver方法。
onDraw方法先于drawChildren，
onDrawOver在drawChildren之后，
一般我们选择复写其中一个即可。
getItemOffsets可以通过outRect.set()为每个Item
设置一定的偏移量，主要用于绘制Decorator。
DividerItemDecoration该类很好的实现了
RecyclerView添加分割线（当使用LayoutManager
为LinearLayoutManager时）。
该实现类可以看到通过读取系统主题中的 
android.R.attr.listDivider作为Item间的分割线，
并且支持横向和纵向。
获取到listDivider以后，该属性的值是个Drawable，
在getItemOffsets中，outRect去设置了绘制的范围。
onDraw中实现了真正的绘制。
我们在原来的代码中添加一句：
mRecyclerView.addItemDecoration(
new DividerItemDecoration(this,
DividerItemDecoration.VERTICAL_LIST));
该分割线是系统默认的，你可以在theme.xml中
找到该属性的使用情况。那么，使用系统的
listDivider有什么好处呢？就是方便我们去随意的改变，
该属性我们可以直接声明在：
<!-- Application theme. -->
<style name="AppTheme" parent="AppBaseTheme">
	<item name="android:listDivider">@drawable/divider_bg</item>  
</style>
然后自己写个drawable即可，下面我们换一种分隔符：
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android"
    android:shape="rectangle" >
    <gradient
        android:centerColor="#ff00ff00"
        android:endColor="#ff0000ff"
        android:startColor="#ffff0000"
        android:type="linear" />
    <size android:height="4dp"/>
</shape>

LayoutManager
RecyclerView.LayoutManager吧，
这是一个抽象类，好在系统提供了3个实现类：
LinearLayoutManager 现行管理器，支持横向、纵向。
GridLayoutManager 网格布局管理器。
StaggeredGridLayoutManager 瀑布就式布局管理器。
mRecyclerView.setLayoutManager(
new LinearLayoutManager(this));
mRecyclerView.setLayoutManager(
new GridLayoutManager(this,4));
mRecyclerView.setLayoutManager(
new StaggeredGridLayoutManager(
4,        
StaggeredGridLayoutManager.VERTICAL));

ItemAnimator
借助默认的实现，当Item添加和移除的时候，
添加动画效果很简单:
// 设置item动画
mRecyclerView.setItemAnimator(
new DefaultItemAnimator());
注意，这里更新数据集不是用
adapter.notifyDataSetChanged()而是 
notifyItemInserted(position)与
notifyItemRemoved(position)。
public void addData(int position) {
    mDatas.add(position, "Insert One");
    notifyItemInserted(position);
}
public void removeData(int position) {
    mDatas.remove(position);
    notifyItemRemoved(position);
}
Activity中点击MenuItem触发：
@Override
public boolean onCreateOptionsMenu(Menu menu)
{
    getMenuInflater().inflate(R.menu.main, menu);
    return super.onCreateOptionsMenu(menu);
}

@Override
public boolean onOptionsItemSelected(MenuItem item)
{
    switch (item.getItemId())
    {
    case R.id.id_action_add:
        mAdapter.addData(1);
        break;
    case R.id.id_action_delete:
        mAdapter.removeData(1);
        break;
    }
    return true;
}

Click and LongClick
系统没有提供ClickListener和LongClickListene。
class HomeAdapter extends RecyclerView.Adapter<HomeAdapter.MyViewHolder>
{

//...
    public interface OnItemClickLitener
    {
        void onItemClick(View view, int position);
        void onItemLongClick(View view , int position);
    }

    private OnItemClickLitener mOnItemClickLitener;

    public void setOnItemClickLitener(OnItemClickLitener mOnItemClickLitener)
    {
        this.mOnItemClickLitener = mOnItemClickLitener;
    }

    @Override
    public void onBindViewHolder(final MyViewHolder holder, final int position)
    {
        holder.tv.setText(mDatas.get(position));

        // 如果设置了回调，则设置点击事件
        if (mOnItemClickLitener != null)
        {
            holder.itemView.setOnClickListener(new OnClickListener()
            {
                @Override
                public void onClick(View v)
                {
                    int pos = holder.getLayoutPosition();
                    mOnItemClickLitener.onItemClick(holder.itemView, pos);
                }
            });

            holder.itemView.setOnLongClickListener(new OnLongClickListener()
            {
                @Override
                public boolean onLongClick(View v)
                {
                    int pos = holder.getLayoutPosition();
                    mOnItemClickLitener.onItemLongClick(holder.itemView, pos);
                    return false;
                }
            });
        }
    }
//...
}
adapter中自己定义了个接口，然后
在onBindViewHolder中去为holder.itemView去设置相应 
的监听最后回调我们设置的监听。
<?xml version="1.0" encoding="utf-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android" >
    <item android:state_pressed="true" android:drawable="@color/color_item_press"></item>
    <item android:drawable="@color/color_item_normal"></item>
</selector>

RecyclerView滚动定位
RecyclerView提供的用于控制移动的方法有2个 
- scrollToPosition(int) 
这个方法的作用是显示指定项，就是把你想置顶的项
显示出来，但是在屏幕的什么位置是不管的，只要那
一项现在看得到了，那它就罢工了！ 
- scrollBy(int x,int y) 
这个方法是自己去控制移动的距离，单位应该是像素。
使用scrollToPosition时，移动到前面的项时，它默认
会将要显示的项置顶，但是移动到后面的项时，位置就
说不好了（看它心情！）一般会显示在最后一个。用过
的应该都有所了解。 使用scrollBy时就需要自己计算
高度或者宽度。在动态的布局中且各项样式高度可能
都不一样的情况下，自己计算高度是很有难度的。
上面说了这么多废话，结论就是这2个方法都不能很好
解决问题，但是，当他们2个结合使用的时候，我们的
问题就变的好解决很多了！
思路是：先用scrollToPosition，将要置顶的项先移动
显示出来，然后计算这一项离顶部的距离，用scrollBy
完成最后的100米！

RecycleView4种定位
可以用 RecyclerView的layoutManager调用下边的方法 
mLayoutManager.scrollToPositionWithOffset(
newActiveViewPosition+1,0);
滑动到的位置和屏幕顶部是一齐的。
mRecyclerView.smoothScrollToPosition(position);
RecyclerView这个方法调用之后底部和position是对齐的。
滚动到指定位置
recyclerView.scrollToPosition(position);
平滑滚动到指定位置
recyclerView.smoothScrollToPosition(position);

RecyclerView实现侧滑删除和长按拖拽
和ListView不同的是，这里必须写这一句，
不然View显示不出来，也是这一句话可以控制布局为
LinearLayout或者是GridView或者是瀑布流布局
rv.setLayoutManager(new LinearLayoutManager(this)); 
//为RecycleView绑定触摸事件  
ItemTouchHelper helper = new ItemTouchHelper(new ItemTouchHelper.Callback() {  
    @Override  
    public int getMovementFlags(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder) {  
        //首先回调的方法 返回int表示是否监听该方向  
        int dragFlags = ItemTouchHelper.UP|ItemTouchHelper.DOWN;//拖拽  
        int swipeFlags = ItemTouchHelper.LEFT|ItemTouchHelper.RIGHT;//侧滑删除  
        return makeMovementFlags(dragFlags,swipeFlags);  
    }  
  
    @Override  
    public boolean onMove(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder, RecyclerView.ViewHolder target) {  
        //滑动事件  
        Collections.swap(list,viewHolder.getAdapterPosition(),target.getAdapterPosition());  
        adapter.notifyItemMoved(viewHolder.getAdapterPosition(),target.getAdapterPosition());  
        return false;  
    }  
  
    @Override  
    public void onSwiped(RecyclerView.ViewHolder viewHolder, int direction) {  
        //侧滑事件  
        list.remove(viewHolder.getAdapterPosition());  
        adapter.notifyItemRemoved(viewHolder.getAdapterPosition());  
    }  
  
    @Override  
    public boolean isLongPressDragEnabled() {  
        //是否可拖拽  
        return true;  
    }  
});  
helper.attachToRecyclerView(rv);  
思路分析：我们规定拖拽的动作是上下拖拽，
而删除效果是左右删除。在拖拽事件中，
就是通过集合的一个交换然后实现布局的更新，
这里RecyclerView的adapter都帮我们实现好了，
用起来很简单，而删除事件就是集合删除一个数据，
然后更新布局。
如果要实现效果二，可以做出类似桌面拖拽排序的效果，
这个就请大家自由发挥吧，实现效果二，只要在效果一
的基础上修改两处代码即可：
// rv.setLayoutManager(new LinearLayoutManager(this));  
rv.setLayoutManager(new GridLayoutManager(this,2)); 
//首先回调的方法 返回int表示是否监听该方向  
int dragFlags = ItemTouchHelper.UP
|ItemTouchHelper.DOWN
|ItemTouchHelper.LEFT
|ItemTouchHelper.RIGHT;//拖拽  
int swipeFlags = 0;//侧滑删除  
return makeMovementFlags(dragFlags,swipeFlags);

RecyclerView.Adapter<RecyclerView.ViewHolder>的子类 
一般覆写四个方法
1.public int getItemViewType(int position) {
	根据位置返回不同的类型
}
2.public RecyclerView.ViewHolder onCreateViewHolder(
final ViewGroup parent, int viewType) {
	不同的类型对应不同的ViewHolder,根据viewType创建
	不同的ViewHolder。
} 
3.public void onBindViewHolder(
RecyclerView.ViewHolder holder, int position) {}
4.public int getItemCount() {}
上面的parent就是RecyclerView,如果需要使用到
RecyclerView，那么强转一下就行了。
*/