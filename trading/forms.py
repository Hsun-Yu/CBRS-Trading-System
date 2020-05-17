from django import forms
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm
from .models import PAL


class SignUpForm(UserCreationForm):
    userSelect = forms.ChoiceField(choices=[(1, "GAA"), (2, "PAL")])
    preferenceSelect = forms.ModelChoiceField(queryset=PAL.objects.all(), required=False)

    class Meta:
        model = User
        fields = ('username', 'password1', 'password2', 'userSelect', 'preferenceSelect', )