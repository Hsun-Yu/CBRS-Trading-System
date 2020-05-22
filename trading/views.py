from django.shortcuts import render
from django.http import HttpResponse
from .forms import SignUpForm
from django.contrib.auth.models import User, Group
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth import login, logout, authenticate
from .models import PAL, GAA
from django.shortcuts import render, redirect
from django.contrib import messages
from django.contrib.auth import login as auth_login
from django.views.generic import TemplateView
from chartjs.views.lines import BaseLineChartView

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
                pal = PAL.objects.create(user=user)
                PAL.save(pal)
            messages.success(request, f"{username} Sign Up Success~ You Can Login Now~")
            return redirect('trading/login')
    else:
        form = SignUpForm()
    return render(request, 'trading/signup.html', {'form': form})

def logoutView(request):
    logout(request)
    messages.info(request, "Logged out successfully!")
    return redirect('/')

def setPriceView(request):
    return render(request, 'trading/setprice.html')


# Default colour for chart
COLORS = [
    (255, 99, 132),
    (54, 162, 235),
    (255, 106, 235),
    (255, 206, 86),
    (75, 192, 192),
    (153, 102, 255),
    (255, 159, 64),
    (166, 133, 93),  # Light brown
    (75, 64, 191),  # Red blue
    (237, 124, 60),  # orange
]

def next_color(color_list=COLORS):
    """Create a different color from a base color list.

    >>> color_list = (
    ...    (122, 159, 191),  # Light blue
    ...    (202, 201, 197),  # Light gray,
    ... )
    >>> g = next_color(color_list)
    >>> next(g)
    [122, 159, 191]
    >>> next(g)
    [202, 201, 197]
    >>> next(g)
    [63, 100, 132]
    >>> next(g)
    [143, 142, 138]
    >>> next(g)
    [4, 41, 73]
    >>> next(g)
    [84, 83, 79]
    """
    step = 0
    while True:
        for color in color_list:
            yield list(map(lambda base: (base + step) % 256, color))
        step += 197


class LineChartJSONView(BaseLineChartView):
    def get_labels(self):
        """Return 7 labels for the x-axis."""
        return ["January", "February", "March", "April", "May", "June", "July"]

    def get_providers(self):
        """Return names of datasets."""
        return ["Central", "Eastside", "Westside", "HAHHA"]

    def get_data(self):
        """Return 3 datasets to plot."""

        return [[75, 44, 92, 11, 44, 95, 35],
                [41, 92, 18, 3, 73, 87, 92],
                [87, 21, 94, 3, 90, 13, 65],
                [10, 20, 30, 40, 50, 60, 70]]
    
    # Rewrite the default colours
    def get_colors(self):
        return next_color()
    
    # Rewrite the default colours setting
    def get_dataset_options(self, index, color):
        default_opt = {
            "backgroundColor": "rgba(%d, %d, %d, 0.2)" % color,
            "borderColor": "rgba(%d, %d, %d, 1)" % color,
            "pointBackgroundColor": "rgba(%d, %d, %d, 1)" % color,
            "pointBorderColor": "#fff",
        }
        return default_opt

line_chart = TemplateView.as_view(template_name='line_chart.html')
line_chart_json = LineChartJSONView.as_view()