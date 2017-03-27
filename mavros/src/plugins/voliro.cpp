#include <mavros/mavros_plugin.h>
#include <mavros_msgs/voliro_ao.h>
#include <mavros_msgs/voliro_alpha.h>

namespace mavros {
namespace std_plugins {


/**
 * @brief IMU data publication plugin
 */
class VOLIROPubPlugin : public plugin::PluginBase {
public:
	VOLIROPubPlugin() : PluginBase(),
		volo_nh("~voliro"),
		has_voliro(false),
		has_voliro_alpha(false)
	{ }

	void initialize(UAS &uas_)
	{
		PluginBase::initialize(uas_);

		voliro_pub = volo_nh.advertise<mavros_msgs::voliro_ao>("voliro_ao",10);
		voliro_alpha_pub = volo_nh.advertise<mavros_msgs::voliro_alpha>("voliro_alpha",10);

		// reset has_* flags on connection change
		enable_connection_cb();
	}

	Subscriptions get_subscriptions() {
		return {
           make_handler(&VOLIROPubPlugin::handle_voliro),
					 make_handler(&VOLIROPubPlugin::handle_voliro_alpha),
		};
	}

private:
	ros::NodeHandle volo_nh;
	ros::Publisher voliro_pub;
	ros::Publisher voliro_alpha_pub;

	bool has_voliro;
	bool has_voliro_alpha;

	void handle_voliro(const mavlink::mavlink_message_t *msg, mavlink::common::msg::VOLIRO_AO &volao)
	{

		ROS_INFO_COND_NAMED(!has_voliro, "voliro", "VOLIRO: VOLIRO ALPHA/OMEGA detected!");
		has_voliro = true;

		//ROS_INFO("VOLIRO WAS HERE");

		mavros_msgs::voliro_ao volomsg;

		volomsg.alpha[0] = volao.alpha[0];
		volomsg.alpha[1] = volao.alpha[1];
		volomsg.alpha[2] = volao.alpha[2];
		volomsg.alpha[3] = volao.alpha[3];
		volomsg.alpha[4] = volao.alpha[4];
		volomsg.alpha[5] = volao.alpha[5];

		volomsg.omega[0] = volao.omega[0];
		volomsg.omega[1] = volao.omega[1];
		volomsg.omega[2] = volao.omega[2];
		volomsg.omega[3] = volao.omega[3];
		volomsg.omega[4] = volao.omega[4];
		volomsg.omega[5] = volao.omega[5];

		voliro_pub.publish(volomsg);

		// mavros_msgs::voliro_ao volomsg;
		// volomsg.alpha[0] = 5.0;
		//
		// voliro_pub.publish(volomsg);


	}

	void handle_voliro_alpha(const mavlink::mavlink_message_t *msg, mavlink::common::msg::VOLIRO_ALPHA &vola)
	{

		ROS_INFO_COND_NAMED(!has_voliro_alpha, "voliro", "VOLIRO: VOLIRO ALPHA detected!");
		has_voliro_alpha = true;

		//ROS_INFO("VOLIRO WAS HERE");

		mavros_msgs::voliro_alpha volo_alpha_msg;

		volo_alpha_msg.alpha[0] = vola.alpha[0];
		volo_alpha_msg.alpha[1] = vola.alpha[1];
		volo_alpha_msg.alpha[2] = vola.alpha[2];
		volo_alpha_msg.alpha[3] = vola.alpha[3];
		volo_alpha_msg.alpha[4] = vola.alpha[4];
		volo_alpha_msg.alpha[5] = vola.alpha[5];

		voliro_alpha_pub.publish(volo_alpha_msg);
	}

	void connection_cb(bool connected) override
	{
    has_voliro = false;
	}

};
}	// namespace std_plugins
}	// namespace mavros

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::std_plugins::VOLIROPubPlugin, mavros::plugin::PluginBase)
