# ComputationalFinance

Developing a library to price options. 

## Summary
_Goals_:
- scalability
- ability to quickly price different contracts

_Current can price_:
- Plain vanilla options
- Up and out barrier options

_This library includes pricing techniques using_:
- closed form solutions (Black-Scholes and variants, up and out closed form solution)
- analytical solutions (Monte Carlo simulation with Markov price process)
- numerical approximations (Euler, Milstein)

_Efforts to improve on Monte Carlo simulations include implementing_:
- Antithetic variants to reduce number of paths to generate (and deflate the variance)
- Control Variate technique to deflate the variance

_Random Number Generation_:
Implementation for the following PRNGs are included:
*Uniform*
- Park Miller uniform random number generator (Linear congruential generator)
- L'Ecuyer uniform random number generator (combined Linear congruential generator)

*Normal*
- Inverse Transform (Joshi's code)
- Fishman acceptance-rejection algorithm
- Box Muller Marsaglia (Joshi's code mixed with my own generators)

## Important Code Structure
Much of the implementation is done through use of general interfaces I designed. This is in an effort to
improve scalability and transparency of the methods in OptionPricer.h. Current interfaces are as follows:
**Generator.h, Asset.h, Option.h, NumericalMethod.h, StochasticNumericalMethod.h**

See these files to see how the derived classes should be used. Because of the structure of these interfaces, it is easy to modify them without have to change much (or at all) the OptionPricer.h class to include new methods or options to price. 
