from django.shortcuts import render
from django.http import HttpResponse
from .forms import SignUpForm
from django.contrib.auth.models import User
from .models import PAL, GAA
from django.shortcuts import render, redirect

# Create your views here.
def index(request):
    return render(request, 'trading/index.html')

def login(request):
    return render(request, 'trading/login.html')

def signup(request):
    if request.method == 'POST':
        form = SignUpForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            user = authenticate(username=username, password=raw_password)
            login(request, user)
            return redirect('home')
    else:
        form = SignUpForm()
    return render(request, 'trading/signup.html', {'form': form})