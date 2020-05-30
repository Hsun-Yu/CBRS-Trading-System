from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from .forms import SignUpForm, EditPALForm
from django.contrib.auth.models import User, Group
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth import login, logout, authenticate
from .models import PAL, GAA, PALHistory
from django.shortcuts import render, redirect
from django.contrib import messages
from django.contrib.auth import login as auth_login
from django.views.generic import TemplateView

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
                messages.success(request, f"{username} Logged In successfully!")
                return redirect('/')
            else:
                messages.warning(request, "Password or Username Wrong!!")
        else:
            messages.warning(request, "Password or Username Wrong!!")

    form = AuthenticationForm()
    return render(request = request, template_name = "trading/login.html", context={"form":form})

def signupView(request):
    if request.method == 'POST':
        form = SignUpForm(request.POST)
        if form.is_valid():
            user = form.save()
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            userType = form.cleaned_data.get('userSelect')
            preference = form.cleaned_data.get('preferenceSelect')
            numberOfRemaining = form.cleaned_data.get('numberOfRemaining')
            price = form.cleaned_data.get('price')
            # Sign up GAA
            if userType == '1':
                group = Group.objects.get(name='GAA')
                user.groups.add(group)
                gaa = GAA.objects.create(user=user, preference=preference)
                GAA.save(gaa)
            # Sign up PAL
            else:
                group = Group.objects.get(name='PAL')
                user.groups.add(group)
                pal = PAL.objects.create(user=user, 
                                        numberOfRemaining=numberOfRemaining,
                                        price=price)
                PAL.save(pal)
                plahistory = PALHistory.objects.create(PAL=pal, 
                                                        price=price)
            messages.success(request, f"{username} Signed up successfully~ You Can Login Now~")
            return redirect('trading/login')
    else:
        form = SignUpForm()
    return render(request, 'trading/signup.html', {'form': form})

def logoutView(request):
    logout(request)
    messages.info(request, "Logged out successfully!")
    return redirect('/')

def setPriceView(request):
    if request.method == 'POST':
        form = EditPALForm(request.POST)
        if form.is_valid():
            pal = PAL.objects.get(user=request.user)
            originalPrice = pal.price
            pal.price = form.cleaned_data.get('price')
            pal.numberOfRemaining = form.cleaned_data.get('numberOfRemaining')
            pal.save()
            history = PALHistory.objects.create(PAL=pal, price=pal.price)
            PALHistory.save(history)
            messages.success(request, f"Edited successfully~")
            return HttpResponseRedirect('/trading/setprice')


    if request.user.is_authenticated:
        user = request.user
        if PAL.objects.filter(user=user).exists():
            pal = PAL.objects.get(user=user)
            price = PALHistory.objects.filter(PAL=pal)
            data = []
            labels = []
            label = str(request.user.username)
            for i in price:
                data.append((int)(i.price))
                labels.append(i.updateDateTime.strftime('%Y-%m-%d %H:%M'))

            form = EditPALForm(instance=pal)
            return render(request, 'trading/setprice.html', {'data':data, 'labels':labels, 'label':label, 'pal':pal, 'form':form})
    return redirect('/')