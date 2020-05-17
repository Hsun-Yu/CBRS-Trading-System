from django import forms
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm
from .models import PAL


class SignUpForm(UserCreationForm):
    preferenceSelect = forms.ModelChoiceField(queryset=PAL.objects.all())

    class Meta:
        model = User
        fields = ('username', 'password1', 'password2', 'preferenceSelect', )