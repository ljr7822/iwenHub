# from django.conf.urls import url
#
# from . import views
#
# urlpatterns=[
#     url(r'^$', views.index),
#     url(r'^add/$', views.add),
# ]


from django.urls import path
from . import views

app_name = 'studentsApp'
urlpatterns = [
    path('index/', views.index, name='index'),
    path('login/', views.login, name='login'),
    path('logout/', views.logout, name='logout'),
    path('add/', views.add, name='add'),
    path('select/', views.select, name='select'),
    path('delete/', views.delete, name='delete'),
    path('update/', views.update, name='update')
]