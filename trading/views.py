from django.shortcuts import render
from django.http import HttpResponse
from .forms import SignUpForm
from django.contrib.auth.models import User
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth import login, logout, authenticate
from .models import PAL, GAA
from django.shortcuts import render, redirect
from django.contrib import messages
from django.contrib.auth import login as auth_login

# Create your views here.
def index(request):
    return render(request, 'trading/index.html')

def loginView(request):
    if request.method == 'POST':
        form = AuthenticationForm(request=request, data=request.POST)
        if form.is_valid():
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password')
            user = authenticate(username=username, password=password)
            if user is not None:
                login(request, user)
                messages.info(request, f"{username} Login Success~")
                return redirect('/')
            else:
                messages.info(request, "Password or Username Wrong!!")
        else:
            messages.info(request, "Password or Username Wrong!!")

    form = AuthenticationForm()
    return render(request = request, template_name = "trading/login.html", context={"form":form})

def signupView(request):
    if request.method == 'POST':
        form = SignUpForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            userType = form.cleaned_data.get('userSelect')
            preference = form.cleaned_data.get('preferenceSelect')
            user = User.objects.get(username=username)
            if userType == '1':
                gaa = GAA.objects.create(user=user, preference=preference)
                GAA.save(gaa)
            else:
                pal = PAL.objects.create(user=user)
                PAL.save(pal)
            return redirect('trading/login')
    else:
        form = SignUpForm()
    return render(request, 'trading/signup.html', {'form': form})