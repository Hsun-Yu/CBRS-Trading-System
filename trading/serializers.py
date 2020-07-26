from django.contrib.auth.models import User, Group
from .models import PAL, PALHistory, GAA, Order, GAAState
from rest_framework import serializers

class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ('url', 'username', 'email', 'groups')

class GroupSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Group
        fields = ('url', 'name')

class PALSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = PAL
        fields = ['url', 'user', 'numberOfRemaining', 'price']

class PALHistorySerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = PALHistory
        fields = ['url', 'PAL', 'price', 'updateDateTime']

class GAAStateSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = GAAState
        fields = ['url', 'name']

class GAASerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = GAA
        fields = ['url', 'user', 'status', 'preference']

class OrderSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Order
        fields = ['url', 'GAA', 'PAL', 'price', 'orderDatetime', 'isFinish', 'dealDateTime']