#ifndef __AXLE_H__
#define __AXLE_H__

#include "lion/frame/frame.h"
#include "lion/io/Xml_document.h"

//!         A generic axle class
//!         --------------------
//! The generic axle class is responsible of the storage and interface with the tire objects
//!  @param Tires_tuple: must be a tuple with the types of the tires 
//!                      (1 for motorcycles/2 for cars)
//!  @param STATE0: index of the first state variable defined here
//!  @param CONTROL0: index of the first control variable defined here
template <typename Timeseries_t,typename Tires_tuple, size_t STATE0, size_t CONTROL0>
class Axle
{
 public:
    using Timeseries_type = Timeseries_t;

    //! Number of tires
    constexpr static size_t NTIRES = std::tuple_size<Tires_tuple>::value;

    //! Indices of the state variables of this class: none
    enum State     { STATE_END    = STATE0    } ;

    //! Indices of the control variables of this class: none
    enum Controls  { CONTROL_END  = CONTROL0  } ;

    //! Default constructor
    Axle() = default;

    //! Constructor
    //! 
    //! @param[in] name: name of the axle
    //! @param[in] tires: tuple containing the tires
    Axle(const std::string& name, const Tires_tuple& tires);

    //! Copy assignment operator
    //! @param[in] other: axle to copy
    Axle& operator=(const Axle& other);

    //! Copy constructor operator
    //! @param[in] other: axle to copy
    Axle(const Axle& other);

    template<typename T>
    bool set_parameter(const std::string& parameter, const T value);

    //! Get a reference to the axle frame
    Frame<Timeseries_t>& get_frame() { return _frame; }

    //! Get a const reference to the frame
    const Frame<Timeseries_t>& get_frame() const { return _frame; }

    //! Get a const reference to one of the tires
    //! @param I: position of the tire in the tuple
    template<size_t I>
    const typename std::tuple_element<I,Tires_tuple>::type& get_tire() const 
        { return std::get<I>(_tires); }

    //! Get the force acting on the axle center [N]
    const Vector3d<Timeseries_t>& get_force() const { return _F; }

    //! Get the torque acting on the axle center [Nm]
    const Vector3d<Timeseries_t>& get_torque() const { return _T; } 

    //! Load the time derivative of the state variables computed herein to the dqdt
    //! @param[out] dqdt: the vehicle state vector time derivative
    template<size_t N>
    void get_state_derivative(std::array<Timeseries_t,N>& dqdt) const;

    //! Set the state variables of this class
    //! @param[in] q: the vehicle state vector 
    //! @param[in] u: the vehicle control vector
    template<size_t NSTATE, size_t NCONTROL>
    void set_state_and_controls(const std::array<Timeseries_t,NSTATE>& q, 
                                const std::array<Timeseries_t,NCONTROL>& u);

    //! Set the state and controls upper, lower, and default values
    template<size_t NSTATE, size_t NCONTROL>
    void set_state_and_control_upper_lower_and_default_values(std::array<scalar,NSTATE>& q_def,
                                                               std::array<scalar,NSTATE>& q_lb,
                                                               std::array<scalar,NSTATE>& q_ub,
                                                               std::array<scalar,NCONTROL>& u_def,
                                                               std::array<scalar,NCONTROL>& u_lb,
                                                               std::array<scalar,NCONTROL>& u_ub 
                                                              ) const;

    //! Get the names of the state and control varaibles of this class
    //! @param[out] q: the vehicle state names
    //! @param[out] u: the vehicle control names
    template<size_t NSTATE, size_t NCONTROL>
    static void set_state_and_control_names(std::array<std::string,NSTATE>& q, 
                                     std::array<std::string,NCONTROL>& u);

    static std::string type() { return "axle"; }
    
    void fill_xml(Xml_document& doc) const;
 private:
    Frame<Timeseries_t> _frame; //! Frame<Timeseries_t> on the axle center

 protected:
    std::string _name;    //! Axle name (e.g. front, rear)
    std::string _path;    //! Axle path in the database file
    Tires_tuple _tires;   //! Tuple containing the tires

    Vector3d<Timeseries_t> _F;   //! [out] Forces generated by the tires
    Vector3d<Timeseries_t> _T;   //! [out] Torque generated by the tires
};

#include "axle.hpp"

#endif
