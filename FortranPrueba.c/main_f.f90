module get_c_float_array
   use iso_c_binding
contains
   subroutine print_float_array(Array, dimension) bind(C)
      INTEGER(C_INT), INTENT(in), VALUE :: dimension
      REAL(C_FLOAT), INTENT(INOUT) :: Array(dimension)
      write (*, *) Array(:)
   end subroutine
end module
