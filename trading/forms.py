from django import forms
from django.forms import ModelForm
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm
from .models import PAL, GAA

class SignUpForm(UserCreationForm):
    userSelect = forms.ChoiceField(choices=[(1, "GAA"), (2, "PAL")])
    preferenceSelect = forms.ModelChoiceField(queryset=PAL.objects.all(), required=False)
    numberOfRemaining = forms.IntegerField(initial=1, min_value=1)
    price = forms.DecimalField(initial=500)
    class Meta:
        model = User
        fields = ('username', 'password1', 'password2', 'userSelect', 'preferenceSelect', 'numberOfRemaining', 'price')

class EditPALForm(ModelForm):
    class Meta:
        model = PAL
        exclude = ()
        fields = ('numberOfRemaining', 'price')

class EditGAAForm(ModelForm):
    class Meta:
        model = GAA
        exclude = ()
        fields = ('preference',)
        