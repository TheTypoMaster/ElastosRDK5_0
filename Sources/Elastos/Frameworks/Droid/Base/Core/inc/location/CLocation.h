
#ifndef __CLOCATION_H__
#define __CLOCATION_H__

#include "ext/frameworkext.h"
#include "_CLocation.h"

using Elastos::Droid::Os::IBundle;
struct ElaLocationCallback
{
    Double (*getLatitude)(IInterface* obj);
    Double (*getLongitude)(IInterface* obj);
    Boolean (*hasAltitude)(IInterface* obj);
    Double (*getAltitude)(IInterface* obj);
    Boolean (*hasAccuracy)(IInterface* obj);
    Float (*getAccuracy)(IInterface* obj);
    Boolean (*hasBearing)(IInterface* obj);
    Float (*getBearing)(IInterface* obj);
    Boolean (*hasSpeed)(IInterface* obj);
    Float (*getSpeed)(IInterface* obj);
    Int64 (*getTime)(IInterface* obj);
};

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * A data class representing a geographic location.
 *
 * <p>A location can consist of a latitude, longitude, timestamp,
 * and other information such as bearing, altitude and velocity.
 *
 * <p>All locations generated by the {@link LocationManager} are
 * guaranteed to have a valid latitude, longitude, and timestamp
 * (both UTC time and elapsed real-time since boot), all other
 * parameters are optional.
 */
CarClass(CLocation)
{
public:
    CLocation();

    CARAPI constructor();

    /**
     * Construct a new Location with a named provider.
     *
     * <p>By default time, latitude and longitude are 0, and the location
     * has no bearing, altitude, speed, accuracy or extras.
     *
     * @param provider the name of the provider that generated this location
     */
    CARAPI constructor(
        /* [in] */ const String& provider);

    /**
     * Construct a new Location object that is copied from an existing one.
     */
    CARAPI constructor(
        /* [in] */ ILocation* l);

    /**
     * Sets the contents of the location to the values from the given location.
     */
    CARAPI Set(
        /* [in] */ ILocation* l);

    /**
     * Clears the contents of the location.
     */
    CARAPI Reset();

    /**
     * Converts a coordinate to a String representation. The outputType
     * may be one of FORMAT_DEGREES, FORMAT_MINUTES, or FORMAT_SECONDS.
     * The coordinate must be a valid double between -180.0 and 180.0.
     *
     * @throws IllegalArgumentException if coordinate is less than
     * -180.0, greater than 180.0, or is not a number.
     * @throws IllegalArgumentException if outputType is not one of
     * FORMAT_DEGREES, FORMAT_MINUTES, or FORMAT_SECONDS.
     */
    static CARAPI Convert(
        /* [in] */ Double coordinate,
        /* [in] */ Int32 outputType,
        /* [out] */ String* str);

    /**
     * Converts a String in one of the formats described by
     * FORMAT_DEGREES, FORMAT_MINUTES, or FORMAT_SECONDS into a
     * double.
     *
     * @throws NullPointerException if coordinate is null
     * @throws IllegalArgumentException if the coordinate is not
     * in one of the valid formats.
     */
    static CARAPI Convert(
        /* [in] */ const String& coordinate,
        /* [out] */ Double* val);

    /**
     * Computes the approximate distance in meters between two
     * locations, and optionally the initial and final bearings of the
     * shortest path between them.  Distance and bearing are defined using the
     * WGS84 ellipsoid.
     *
     * <p> The computed distance is stored in results[0].  If results has length
     * 2 or greater, the initial bearing is stored in results[1]. If results has
     * length 3 or greater, the final bearing is stored in results[2].
     *
     * @param startLatitude the starting latitude
     * @param startLongitude the starting longitude
     * @param endLatitude the ending latitude
     * @param endLongitude the ending longitude
     * @param results an array of floats to hold the results
     *
     * @throws IllegalArgumentException if results is null or has length < 1
     */
    static CARAPI DistanceBetween(
        /* [in] */ Double startLatitude,
        /* [in] */ Double startLongitude,
        /* [in] */ Double endLatitude,
        /* [in] */ Double endLongitude,
        /* [in] */ ArrayOf<Float>* results);

    /**
     * Returns the approximate distance in meters between this
     * location and the given location.  Distance is defined using
     * the WGS84 ellipsoid.
     *
     * @param dest the destination location
     * @return the approximate distance in meters
     */
    CARAPI DistanceTo(
        /* [in] */ ILocation* dest,
        /* [out] */ Float* distance);

    /**
     * Returns the approximate initial bearing in degrees East of true
     * North when traveling along the shortest path between this
     * location and the given location.  The shortest path is defined
     * using the WGS84 ellipsoid.  Locations that are (nearly)
     * antipodal may produce meaningless results.
     *
     * @param dest the destination location
     * @return the initial bearing in degrees
     */
    CARAPI BearingTo(
        /* [in] */ ILocation* dest,
        /* [out] */ Float* initialBearing);

    /**
     * Returns the name of the provider that generated this fix.
     *
     * @return the provider, or null if it has not been set
     */
    CARAPI GetProvider(
        /* [out] */ String* provider);

    /**
     * Sets the name of the provider that generated this fix.
     */
    CARAPI SetProvider(
        /* [in] */ const String& provider);

    /**
     * Return the UTC time of this fix, in milliseconds since January 1, 1970.
     *
     * <p>Note that the UTC time on a device is not monotonic: it
     * can jump forwards or backwards unpredictably. So always use
     * {@link #getElapsedRealtimeNanos} when calculating time deltas.
     *
     * <p>On the other hand, {@link #getTime} is useful for presenting
     * a human readable time to the user, or for carefully comparing
     * location fixes across reboot or across devices.
     *
     * <p>All locations generated by the {@link LocationManager}
     * are guaranteed to have a valid UTC time, however remember that
     * the system time may have changed since the location was generated.
     *
     * @return time of fix, in milliseconds since January 1, 1970.
     */
    CARAPI GetTime(
        /* [out] */ Int64* time);

    /**
     * Set the UTC time of this fix, in milliseconds since January 1,
     * 1970.
     *
     * @param time UTC time of this fix, in milliseconds since January 1, 1970
     */
    CARAPI SetTime(
        /* [in] */ Int64 time);

    /**
     * Return the time of this fix, in elapsed real-time since system boot.
     *
     * <p>This value can be reliably compared to
     * {@link android.os.SystemClock#elapsedRealtimeNanos},
     * to calculate the age of a fix and to compare Location fixes. This
     * is reliable because elapsed real-time is guaranteed monotonic for
     * each system boot and continues to increment even when the system
     * is in deep sleep (unlike {@link #getTime}.
     *
     * <p>All locations generated by the {@link LocationManager}
     * are guaranteed to have a valid elapsed real-time.
     *
     * @return elapsed real-time of fix, in nanoseconds since system boot.
     */
    CARAPI GetElapsedRealtimeNanos(
        /* [out] */ Int64* time);

    /**
     * Set the time of this fix, in elapsed real-time since system boot.
     *
     * @param time elapsed real-time of fix, in nanoseconds since system boot.
     */
    CARAPI SetElapsedRealtimeNanos(
        /* [in] */ Int64 time);

    /**
     * Get the latitude, in degrees.
     *
     * <p>All locations generated by the {@link LocationManager}
     * will have a valid latitude.
     */
    CARAPI GetLatitude(
        /* [out] */ Double* latitude);

    /**
     * Set the latitude, in degrees.
     */
    CARAPI SetLatitude(
        /* [in] */ Double latitude);

    /**
     * Get the longitude, in degrees.
     *
     * <p>All locations generated by the {@link LocationManager}
     * will have a valid longitude.
     */
    CARAPI GetLongitude(
        /* [out] */ Double* longitude);

    /**
     * Set the longitude, in degrees.
     */
    CARAPI SetLongitude(
        /* [in] */ Double longitude);

    /**
     * True if this location has an altitude.
     */
    CARAPI HasAltitude(
        /* [out] */ Boolean* result);

    /**
     * Get the altitude if available, in meters above sea level.
     *
     * <p>If this location does not have an altitude then 0.0 is returned.
     */
    CARAPI GetAltitude(
        /* [out] */ Double* altitude);

    /**
     * Set the altitude, in meters above sea level.
     *
     * <p>Following this call {@link #hasAltitude} will return true.
     */
    CARAPI SetAltitude(
        /* [in] */ Double altitude);

    /**
     * Remove the altitude from this location.
     *
     * <p>Following this call {@link #hasAltitude} will return false,
     * and {@link #getAltitude} will return 0.0.
     */
    CARAPI RemoveAltitude();

    /**
     * True if this location has a speed.
     */
    CARAPI HasSpeed(
        /* [out] */ Boolean* result);

    /**
     * Get the speed if it is available, in meters/second over ground.
     *
     * <p>If this location does not have a speed then 0.0 is returned.
     */
    CARAPI GetSpeed(
        /* [out] */ Float* speed);

    /**
     * Set the speed, in meters/second over ground.
     *
     * <p>Following this call {@link #hasSpeed} will return true.
     */
    CARAPI SetSpeed(
        /* [in] */ Float speed);

    /**
     * Remove the speed from this location.
     *
     * <p>Following this call {@link #hasSpeed} will return false,
     * and {@link #getSpeed} will return 0.0.
     */
    CARAPI RemoveSpeed();

    /**
     * True if this location has a bearing.
     */
    CARAPI HasBearing(
        /* [out] */ Boolean* result);

    /**
     * Get the bearing, in degrees.
     *
     * <p>Bearing is the horizontal direction of travel of this device,
     * and is not related to the device orientation. It is guaranteed to
     * be in the range (0.0, 360.0] if the device has a bearing.
     *
     * <p>If this location does not have a bearing then 0.0 is returned.
     */
    CARAPI GetBearing(
        /* [out] */ Float* bearing);

    /**
     * Set the bearing, in degrees.
     *
     * <p>Bearing is the horizontal direction of travel of this device,
     * and is not related to the device orientation.
     *
     * <p>The input will be wrapped into the range (0.0, 360.0].
     */
    CARAPI SetBearing(
        /* [in] */ Float bearing);

    /**
     * Remove the bearing from this location.
     *
     * <p>Following this call {@link #hasBearing} will return false,
     * and {@link #getBearing} will return 0.0.
     */
    CARAPI RemoveBearing();

    /**
     * True if this location has an accuracy.
     *
     * <p>All locations generated by the {@link LocationManager} have an
     * accuracy.
     */
    CARAPI HasAccuracy(
        /* [out] */ Boolean* result);

    /**
     * Get the estimated accuracy of this location, in meters.
     *
     * <p>We define accuracy as the radius of 68% confidence. In other
     * words, if you draw a circle centered at this location's
     * latitude and longitude, and with a radius equal to the accuracy,
     * then there is a 68% probability that the true location is inside
     * the circle.
     *
     * <p>In statistical terms, it is assumed that location errors
     * are random with a normal distribution, so the 68% confidence circle
     * represents one standard deviation. Note that in practice, location
     * errors do not always follow such a simple distribution.
     *
     * <p>This accuracy estimation is only concerned with horizontal
     * accuracy, and does not indicate the accuracy of bearing,
     * velocity or altitude if those are included in this Location.
     *
     * <p>If this location does not have an accuracy, then 0.0 is returned.
     * All locations generated by the {@link LocationManager} include
     * an accuracy.
     */
    CARAPI GetAccuracy(
        /* [out] */ Float* accuracy);

    /**
     * Set the estimated accuracy of this location, meters.
     *
     * <p>See {@link #getAccuracy} for the definition of accuracy.
     *
     * <p>Following this call {@link #hasAccuracy} will return true.
     */
    CARAPI SetAccuracy(
        /* [in] */ Float accuracy);

    /**
     * Remove the accuracy from this location.
     *
     * <p>Following this call {@link #hasAccuracy} will return false, and
     * {@link #getAccuracy} will return 0.0.
     */
    CARAPI RemoveAccuracy();

    /**
     * Return true if this Location object is complete.
     *
     * <p>A location object is currently considered complete if it has
     * a valid provider, accuracy, wall-clock time and elapsed real-time.
     *
     * <p>All locations supplied by the {@link LocationManager} to
     * applications must be complete.
     *
     * @see #makeComplete
     * @hide
     */
    CARAPI IsComplete(
        /* [out] */ Boolean* result);

    /**
     * Helper to fill incomplete fields.
     *
     * <p>Used to assist in backwards compatibility with
     * Location objects received from applications.
     *
     * @see #isComplete
     * @hide
     */
    CARAPI MakeComplete();

    /**
     * Returns additional provider-specific information about the
     * location fix as a Bundle.  The keys and values are determined
     * by the provider.  If no additional information is available,
     * null is returned.
     *
     * <p> A number of common key/value pairs are listed
     * below. Providers that use any of the keys on this list must
     * provide the corresponding value as described below.
     *
     * <ul>
     * <li> satellites - the number of satellites used to derive the fix
     * </ul>
     */
    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    /**
     * Sets the extra information associated with this fix to the
     * given Bundle.
     */
    CARAPI SetExtras(
        /* [in] */ IBundle* extras);

//    @Override
//    String toString();

//    void dump(Printer pw, String prefix);

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    /**
     * Returns one of the optional extra {@link Location}s that can be attached
     * to this Location.
     *
     * @param key the key associated with the desired extra Location
     * @return the extra Location, or null if unavailable
     * @hide
     */
    CARAPI GetExtraLocation(
        /* [in] */ const String& key,
        /* [out] */ ILocation** location);

    /**
     * Attaches an extra {@link Location} to this Location.
     *
     * @param key the key associated with the Location extra
     * @param location the Location to attach
     * @hide
     */
    CARAPI SetExtraLocation(
        /* [in] */ const String& key,
        /* [in] */ ILocation* value);

    static Double GetLatitude(
        /* [in] */ IInterface* obj);

    static Double GetLongitude(
        /* [in] */ IInterface* obj);

    static Boolean HasAltitude(
        /* [in] */ IInterface* obj);

    static Double GetAltitude(
        /* [in] */ IInterface* obj);

    static Boolean HasAccuracy(
        /* [in] */ IInterface* obj);

    static Float GetAccuracy(
        /* [in] */ IInterface* obj);

    static Boolean HasBearing(
        /* [in] */ IInterface* obj);

    static Float GetBearing(
        /* [in] */ IInterface* obj);

    static Boolean HasSpeed(
        /* [in] */ IInterface* obj);

    static Float GetSpeed(
        /* [in] */ IInterface* obj);

    static Int64 GetTime(
        /* [in] */ IInterface* obj);
private:
    static CARAPI ComputeDistanceAndBearing(
        /* [in] */ Double lat1,
        /* [in] */ Double lon1,
        /* [in] */ Double lat2,
        /* [in] */ Double lon2,
        /* [in] */ ArrayOf<Float>* results);

public:
    static struct ElaLocationCallback sElaLocationCallback;

private:
    String mProvider;
    Int64 mTime;
    Int64 mElapsedRealtimeNanos;
    Double mLatitude;
    Double mLongitude;
    Boolean mHasAltitude;
    Double mAltitude;
    Boolean mHasSpeed;
    Float mSpeed;
    Boolean mHasBearing;
    Float mBearing;
    Boolean mHasAccuracy;
    Float mAccuracy;
    AutoPtr<IBundle> mExtras;

    // Cache the inputs and outputs of computeDistanceAndBearing
    // so calls to distanceTo() and bearingTo() can share work
    Double mLat1;
    Double mLon1;
    Double mLat2;
    Double mLon2;
    Float mDistance;
    Float mInitialBearing;
    // Scratchpad
    AutoPtr<ArrayOf<Float> > mResults;
    Mutex mResultsLock;
};

} // namespace Location
} // namepsace Droid
} // namespace Elastos

#endif //__CLOCATION_H__
