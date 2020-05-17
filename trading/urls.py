from django.conf.urls import url
from . import views
urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'/login', views.loginView, name='loginView'),
    url(r'/signup', views.signupView, name='signupView')
];