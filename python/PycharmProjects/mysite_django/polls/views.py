from django.shortcuts import render

# Create your views here.

from django.http import HttpResponse, HttpResponseRedirect, Http404
from django.template import Context, loader
from django.shortcuts import render, get_object_or_404
from django.core.urlresolvers import reverse
from polls.models import Poll, Choice


def index(request):
    # "-pub_date"表示根据"pub_date"字段进行降序排列,如果去掉"-"表示升序
    # latest_poll_list = Poll.objects.order_by("-pub_date")[:5]
    # latest_poll_list = Poll.objects.order_by("-pub_date")
    latest_poll_list = Poll.objects.order_by("pub_date")

    # 第一种方式
    # template = loader.get_template("polls/index.html")
    # html = template.render({
    #     "latest_poll_list": latest_poll_list,
    # })
    # return HttpResponse(html)

    # 第二种方式
    # render()函数中
    # 第一个参数是request对象,
    # 第二个参数是一个模板名称,
    # 第三个是一个字典类型的可选参数。
    return render(
        request,
        "polls/index.html",
        {
            "latest_poll_list": latest_poll_list,
        }
    )

    # output = ", ".join([p.question for p in latest_poll_list])
    # return HttpResponse(output)
    # return HttpResponse("Hello World. You're at the poll index.")


def detail(request, poll_id):
    # 第一种方式
    # try:
    #     poll = Poll.objects.get(pk=poll_id)
    # except Poll.DoesNotExist:
    #     raise Http404
    # return render(
    #     request,
    #     "polls/detail.html",
    #     {
    #         "poll": poll
    #     }
    # )

    # 第二种方式
    poll = get_object_or_404(Poll, pk=poll_id)
    return render(
        request,
        "polls/detail.html",
        {
            "poll": poll
        }
    )
    # return HttpResponse("You're looking at poll %s." % (poll_id))


def results(request, poll_id):
    return HttpResponse("You're looking at the results of poll %s." % (poll_id))


def vote(request, poll_id):
    # return HttpResponse("You're voting on poll %s." % (poll_id))
    p = get_object_or_404(Poll, pk=poll_id)
    try:
        selected_choice = p.choice_set.get(pk=request.POST["choice"])
    except(KeyError, Choice.DoesNotExist):
        return render(
            request,
            "polls/detail.html",
            {
                "poll": p,
                "error_message": "You didn't select a choice",
            }
        )
    else:
        selected_choice.votes += 1
        selected_choice.save()
        return HttpResponseRedirect(reverse("polls:results", args=(p.id,)))
