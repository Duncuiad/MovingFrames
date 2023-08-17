# Moving Frames

> <u>Note</u>: this repository is a *fork* of an earlier repository I made on a server I share with some friends. Even so, I am the sole contributor of that repository

## Overview

The repo is a proof of concept for simple splines of poses (positions and orientations together) implemented using dual quaternions. The core concept is to take the geodesics of the space of unit dual quaternions and use them like they were the straight lines of euclidean space. It turns out that those geodesics are easier to compute than it sounds!

![](Screenshots/Curve02.png)

It turns out that spending some time in creating a micro engine goes a long way in making debugging easier, data creation more straightforward and overall making progress faster. That's how repo ended up with some (very) barebones features like (object-oriented) entities made up of components, world/entity handling, a level editor interface, camera and serialization. I'll surely use them again in future projects.

Here is an example of the interface I used to setup the following pictures. The tripods represent the editable key frames that are then interpolated by the spline algorithms. Each key frame is described by two dual quaternions: one for the pose (orientation + position) and one for the twist (linear and angular velocities).

![](Screenshots/Curve01.png)

The code relies on GLFW for window handling, GLAD (OpenGL 4.5) for graphics, GLM for maths, Assimp for model loading and ImGUI for the graphical interface.

The splines can be used to obtain lots of different effects by making use of geometry shaders:

![](Screenshots/Curve08.png)
![](Screenshots/Curve04.png)
![](Screenshots/Curve05.png)

In the picture above, orange vectors represent angular velocities and light blue vectors represent 'linear' velocities.

## Splines, courtesy of Dual Quaternions

### Geodesics

'Reference frames', 'poses', 'transforms', 'rigid motions' are names we use to describe elements of the group $SE(3)$ or any isomorphic group (we usually care about how these things act on vectors, rarely about how they are represented). Unit dual quaternions are a convienient way of representing them in practice[^1]

Rigid motions can notoriously be decomposed as a rotation about an axis in euclidean space followed by a translation along the same axis (see [Chasles' Theorem (Wikipedia)](https://en.wikipedia.org/wiki/Chasles%27_theorem_(kinematics))). This decomposition is quite convenient when talking about geodesics in $SE(3)$: it turns out that we can see a geodesic path from the identity to any rigid motion as a helicoidal path that gradually rotates and translates along that same axis. This is shown in the following picture:

![](Screenshots/Curve07.png)

The way geodesics between any two frames are computed is quite standard in Lie Theory. In the case of $SE(3)$, I have seen the interpolating geodesic curve being called 'Sclerp' in literature (as in 'SCrew Linear intERPolation'), after the screw motion it attains.

In a Lie group, geodesics between group elements can be parameterized by making use of the exponential and logarithm[^2] maps:

$$
\left( A,B,t \right) \longmapsto \exp\left( t \cdot \log \left( BA^{-1} \right) \right) A
$$

<details>
<summary>$\boxed{\text{Detail}}:\text{What does the formula \textit{mean}?}$</summary>
  Given elements $A$ and $B$ in the group, let's construct a curve from $A$ to $B$, parameterized by $t \in \left[0,1\right]$.
  First, take the group elemet that transforms $A$ into $B$. That is: <br />
  $\textcolor{orange}{BA^{-1}}$ <br />
  Then find the tangent vector which, when followed for $1$ second, brings us from the identity to the element $BA^{-1}$: <br />
  $\textcolor{orange}{\log \left( \textcolor{white}{BA^{-1}} \right)}$ <br />
  Based on the value of $t$, take just the first portion of this tangent vector: <br />
  $\textcolor{orange}{t \cdot} \log \left( BA^{-1} \right)$ <br />
  Now follow this vector along the group for $1$ second (or, equivalently, follow the vector $\log \left( BA^{-1} \right)$ for $t$ seconds: <br />
  $\textcolor{orange}{\exp \left( \textcolor{white}{t \cdot \log \left( BA^{-1} \right)} \right)}$ <br />
  This is now a geodesic curve connecting the identity to $BA^{-1}$. Let's move it from the identity to $A$, so that it connects $A$ to $B$: <br />
    $\exp \left( t \cdot \log \left( BA^{-1} \right) \right) \textcolor{orange}{A}$ <br />
  And there it is!
</details>

The exponential map usually needs to be computed via a costly power series. Luckily, in the case of dual quaternions and rigid motions it can be calculated in closed form. The dual quaternion exponential can be computed in a few equivalent ways, which can be found in literature. The code of this repo implements one of these[^3]

### From geodesics to splines

![](Screenshots/Curve06.png)

[^1]: Keeping in mind that the group of unit dual quaternions 𝔻&#119982;<sup>&#120805;</sup> is actually a double cover of the special euclidean group SE(3)
[^2]: Multiple geodesics between two group elements may exist, in which case the choice of geodesic is determined by the choice of the logarithm branch being used
[^3]: It's worth noting that a formula very similar to Euler's formula for the complex exponential exists. It's more computationally expensive, but it's much more mathematically expressive.
