from django.conf.urls import url
from . import views
from django.urls import path, include
from rest_framework import routers

router = routers.DefaultRouter()
router.register(r'palhistory', views.PALHistoryViewSet)
router.register(r'pal', views.PALViewSet)
router.register(r'user', views.UserViewSet)
router.register(r'group', views.GroupViewSet)
router.register(r'gaastate', views.GAAStateViewSet)
router.register(r'gaa', views.GAAViewSet)
router.register(r'order', views.OrderViewSet)

urlpatterns = [
    url(r'api/pal/', views.PALList.as_view()),
    url(r'api/gaa/', views.GAAList.as_view()),
    url(r'api/ods/', views.OrderList.as_view()),
    path('api', include(router.urls)),
    path('api-auth/', include('rest_framework.urls', namespace='rest_framework')),
    url(r'^$', views.index, name='index'),
    url(r'login', views.loginView, name='loginView'),
    url(r'signup', views.signupView, name='signupView'),
    url(r'logout', views.logoutView, name='logoutView'),
    url(r'price', views.priceView, name='priceView'),
    url(r'order', views.orderView, name='orderView'),
    url(r'market', views.marketView, name='marketView'),
];
