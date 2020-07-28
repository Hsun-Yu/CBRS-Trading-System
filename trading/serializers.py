from django.contrib.auth.models import User, Group
from .models import PAL, PALHistory, GAA, Order, GAAState
from rest_framework import serializers

class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ('url', 'username', 'password',  'groups')

    def create(self, validated_data):
        groups_data = validated_data.pop('groups')
        user = User.objects.create(**validated_data)
        user.set_password(validated_data['password'])
        for group_data in groups_data:
            user.groups.add(group_data)
        User.save(user)
        return user

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